#include "GalaxyCamera.h"

GxCamera::GxCamera(std::string id)
{
    _id = id;
    _init = init();
}

const char *GxCamera::getGxError()
{
    size_t size = 256;
    static char err_info[256];
    GXGetLastError(nullptr, err_info, &size);
    return err_info;
}

bool GxCamera::init()
{
    // 初始化Lib
    auto status = GXInitLib();
    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to init the lib, error: " << getGxError() << std::endl;
        return false;
    }
    // 枚举相机
    uint32_t nums{};
    status = GXUpdateAllDeviceList(&nums, 1000);
    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to update device list, error: " << getGxError() << std::endl;
        return false;
    }
    if (nums == 0)
    {
        std::cerr << "Could not find any camera device" << std::endl;
        return false;
    }
    // 获取设备信息
    std::unordered_set<std::string> mac_set{};
    std::vector<GX_DEVICE_BASE_INFO> device_info(nums);
    size_t base_info_size = nums * sizeof(GX_DEVICE_BASE_INFO);
    status = GXGetAllDeviceBaseInfo(device_info.data(), &base_info_size);
    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to get device info, error: " << getGxError() << std::endl;
        return false;
    }

    // 定义设备打开参数
    GX_OPEN_PARAM open_param;
    open_param.accessMode = GX_ACCESS_EXCLUSIVE;
    open_param.openMode = GX_OPEN_MAC;
    for (int32_t i = 0; i < nums; ++i)
    {
        GX_DEVICE_IP_INFO ip_info;
        if (device_info[i].deviceClass != GX_DEVICE_CLASS_GEV)
        {
            continue;
        }
        status = GXGetDeviceIPInfo(i, &ip_info);
        if (status != GX_STATUS_SUCCESS)
        {
            std::cerr << "Failed to get device IP, error: " << getGxError() << std::endl;
            return false;
        }
        mac_set.emplace(ip_info.szMAC);
    }
    // 打开相机
    if (_id.empty())
    {
        status = GXOpenDeviceByIndex(1, &_handle);
    }
    else if (mac_set.find(_id) != mac_set.end())
    {
        open_param.pszContent = const_cast<char *>(_id.c_str());
        status = GXOpenDevice(&open_param, &_handle);
    }
    else
    {
        std::cerr << "Could not find the device matched with the provided MAC" << std::endl;
        return false;
    }

    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to open the camera, error: " << getGxError() << std::endl;
        return false;
    }
    else
    {
        // 设置相机流通道包长属性
        uint32_t packet_size{};
        GXGetOptimalPacketSize(_handle, &packet_size);
        GXSetIntValue(_handle, "GevSCPSPacketSize", packet_size);
    }
    // 设置工作模式为连续模式
    GXSetEnum(_handle, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
    GXSetEnum(_handle, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
    // 设置曝光模式
    GXSetEnum(_handle, GX_ENUM_EXPOSURE_MODE, GX_EXPOSURE_MODE_TIMED);
    GXSetEnum(_handle, GX_ENUM_EXPOSURE_TIME_MODE, GX_EXPOSURE_TIME_MODE_STANDARD);
    // 取流
    status = GXGetInt(_handle, GX_INT_PAYLOAD_SIZE, &_payload);
    if (status != GX_STATUS_SUCCESS || _payload <= 0)
    {
        std::cerr << "Failed to get payload size, error: " << getGxError() << std::endl;
        return false;
    }
    _data.pImgBuf = new char[_payload];
    status = GXSendCommand(_handle, GX_COMMAND_ACQUISITION_START);
    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to start stream, error: " << getGxError() << std::endl;
        return false;
    }
    _init = true;

    return true;
}
bool GxCamera::retrieve(cv::OutputArray image)
{
    //获取像素格式、图像宽高、图像缓冲区
    int32_t pixel_format = _data.nPixelFormat;
    int32_t width = _data.nWidth, height = _data.nHeight;
    void *buffer = _data.pImgBuf;
    //解码与转码
    if (pixel_format == GX_PIXEL_FORMAT_MONO8)
    {
        image.assign(cv::Mat(height, width, CV_8UC1, buffer));
    }
    else if (pixel_format == GX_PIXEL_FORMAT_BAYER_GR8 || pixel_format == GX_PIXEL_FORMAT_BAYER_RG8 || pixel_format == GX_PIXEL_FORMAT_BAYER_GB8 || pixel_format == GX_PIXEL_FORMAT_BAYER_BG8)
    {
        cv::Mat src_img(height, width, CV_8U, buffer);
        cv::Mat dst_img;
        const static std::unordered_map<int32_t, int> bayer_map{
            {GX_PIXEL_FORMAT_BAYER_GB8, cv::COLOR_BayerGB2BGR},
            {GX_PIXEL_FORMAT_BAYER_GR8, cv::COLOR_BayerGR2BGR},
            {GX_PIXEL_FORMAT_BAYER_BG8, cv::COLOR_BayerBG2BGR},
            {GX_PIXEL_FORMAT_BAYER_RG8, cv::COLOR_BayerRG2BGR},
        };
        cv::cvtColor(src_img, dst_img, bayer_map.at(pixel_format));
        image.assign(dst_img);
    }
    else
    {
        std::cerr << "Invalid pixel format" << std::endl;
        return false;
    }
    return true;
}

bool GxCamera::read(cv::OutputArray image)
{
    auto status = GXGetImage(_handle, &_data, 1000);
    if (status != GX_STATUS_SUCCESS) 
    {
        std::cerr << "Failed to read image, error: " << getGxError() << std::endl;
        reconnect();
        return false;
    }
    auto flag = retrieve(image);
    return flag;
}


bool GxCamera::reconnect() {
    std::cerr << "camera device reconnect " << std::endl;
    release();
    Sleep(100);
    return init();
}

void GxCamera::release()
{
    auto status = GXSendCommand(_handle, GX_COMMAND_ACQUISITION_STOP);
    if (status != GX_STATUS_SUCCESS) {
        std::cerr << "Failed to stop stream, error: " << getGxError() << std::endl;
    }
    status = GXCloseDevice(_handle);
    if (status != GX_STATUS_SUCCESS)
    {
        std::cerr << "Failed to close camera, error: " << getGxError() << std::endl;
    }
    GXCloseLib();
}

GxCamera::~GxCamera()
{
    release();
}