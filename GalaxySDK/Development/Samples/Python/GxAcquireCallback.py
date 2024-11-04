# version:1.1.2312.9221
import gxipy as gx
import time
from PIL import Image
from ctypes import *
from gxipy.gxidef import *
import numpy
from gxipy.ImageFormatConvert import *

def get_best_valid_bits(pixel_format):
    valid_bits = DxValidBit.BIT0_7
    if pixel_format in (GxPixelFormatEntry.MONO8, GxPixelFormatEntry.BAYER_GR8, GxPixelFormatEntry.BAYER_RG8, GxPixelFormatEntry.BAYER_GB8, GxPixelFormatEntry.BAYER_BG8
                        , GxPixelFormatEntry.RGB8, GxPixelFormatEntry.BGR8, GxPixelFormatEntry.R8, GxPixelFormatEntry.B8, GxPixelFormatEntry.G8):
        valid_bits = DxValidBit.BIT0_7
    elif pixel_format in (GxPixelFormatEntry.MONO10, GxPixelFormatEntry.MONO10_PACKED, GxPixelFormatEntry.BAYER_GR10,
                          GxPixelFormatEntry.BAYER_RG10, GxPixelFormatEntry.BAYER_GB10, GxPixelFormatEntry.BAYER_BG10):
        valid_bits = DxValidBit.BIT2_9
    elif pixel_format in (GxPixelFormatEntry.MONO12, GxPixelFormatEntry.MONO12_PACKED, GxPixelFormatEntry.BAYER_GR12,
                          GxPixelFormatEntry.BAYER_RG12, GxPixelFormatEntry.BAYER_GB12, GxPixelFormatEntry.BAYER_BG12):
        valid_bits = DxValidBit.BIT4_11
    elif pixel_format in (GxPixelFormatEntry.MONO14):
        valid_bits = DxValidBit.BIT6_13
    elif pixel_format in (GxPixelFormatEntry.MONO16):
        valid_bits = DxValidBit.BIT8_15
    return valid_bits

def convert_to_special_pixel_format(raw_image, pixel_format):
    image_convert.set_dest_format(pixel_format)
    valid_bits = get_best_valid_bits(raw_image.get_pixel_format())
    image_convert.set_valid_bits(valid_bits)

    # create out put image buffer
    buffer_out_size = image_convert.get_buffer_size_for_conversion(raw_image)
    output_image_array = (c_ubyte * buffer_out_size)()
    output_image = addressof(output_image_array)

    #convert to pixel_format
    image_convert.convert(raw_image, output_image, buffer_out_size, False)
    if output_image is None:
        print('Pixel format conversion failed')
        return

    return output_image_array, buffer_out_size

def capture_callback_color(raw_image):
    # print height, width, and frame ID of the acquisition image
    print("Frame ID: %d   Height: %d   Width: %d"
            % (raw_image.get_frame_id(), raw_image.get_height(), raw_image.get_width()))

    if raw_image.get_pixel_format() != GxPixelFormatEntry.RGB8:
        rgb_image_array, rgb_image_buffer_length = convert_to_special_pixel_format(raw_image, GxPixelFormatEntry.RGB8)
        if rgb_image_array is None:
            return

        # create numpy array with data from rgb image
        numpy_image = numpy.frombuffer(rgb_image_array, dtype=numpy.ubyte, count=rgb_image_buffer_length). \
            reshape(raw_image.frame_data.height, raw_image.frame_data.width, 3)
    else:
        numpy_image = raw_image.get_numpy_array()

    if numpy_image is None:
        print('Failed to get numpy array from RGBImage')
        return

    # show acquired image
    img = Image.fromarray(numpy_image, 'RGB')
    img.show()

def capture_callback_mono(raw_image):
    # print height, width, and frame ID of the acquisition image
    print("Frame ID: %d   Height: %d   Width: %d"
            % (raw_image.get_frame_id(), raw_image.get_height(), raw_image.get_width()))

    if raw_image.get_pixel_format() not in (GxPixelFormatEntry.MONO8, GxPixelFormatEntry.R8, GxPixelFormatEntry.B8, GxPixelFormatEntry.G8):
        mono_image_array, mono_image_buffer_length = convert_to_special_pixel_format(raw_image, GxPixelFormatEntry.MONO8)
        if mono_image_array is None:
            return
        # create numpy array with data from rgb image
        numpy_image = numpy.frombuffer(mono_image_array, dtype=numpy.ubyte, count=mono_image_buffer_length). \
            reshape(raw_image.frame_data.height, raw_image.frame_data.width)
    else:
        numpy_image = raw_image.get_numpy_array()

    if numpy_image is None:
        print('Failed to get numpy array from RawImage')
        return

    # show acquired image
    img = Image.fromarray(numpy_image, 'L')
    img.show()

def main():
    # print the demo information
    print("")
    print("-------------------------------------------------------------")
    print("Sample to show how to acquire mono or color image by callback "
          "and show acquired image.")
    print("-------------------------------------------------------------")
    print("")
    print("Initializing......")
    print("")

    # create a device manager
    device_manager = gx.DeviceManager()
    dev_num, dev_info_list = device_manager.update_all_device_list()
    if dev_num is 0:
        print("Number of enumerated devices is 0")
        return

    # open the first device
    cam = device_manager.open_device_by_index(1)
    remote_device_feature = cam.get_remote_device_feature_control()

    # get image convert obj
    global image_convert
    image_convert = device_manager.create_image_format_convert()

    # set exposure
    exposure_time_feature = remote_device_feature.get_float_feature("ExposureTime")
    exposure_time_feature.set(10000.0)

    trigger_mode_feature = remote_device_feature.get_enum_feature("TriggerMode")
    if dev_info_list[0].get("device_class") == gx.GxDeviceClassList.USB2:
        # set trigger mode
        trigger_mode_feature.set("On")
    else:
        # set trigger mode and trigger source
        trigger_mode_feature.set("On")

        trigger_source_feature = remote_device_feature.get_enum_feature("TriggerSource")
        trigger_source_feature.set("Software")

    # get data stream
    data_stream = cam.data_stream[0]

    # Register capture callback (Notice: Linux USB2 SDK does not support register_capture_callback)
    pixel_format_value, pixel_format_str = remote_device_feature.get_enum_feature("PixelFormat").get()
    if Utility.is_gray(pixel_format_value):
        data_stream.register_capture_callback(capture_callback_mono)
    else:
        data_stream.register_capture_callback(capture_callback_color)

    # start data acquisition
    cam.stream_on()

    print('<Start acquisition>')
    time.sleep(0.1)

    # Send trigger command
    trigger_software_command_feature = remote_device_feature.get_command_feature("TriggerSoftware")
    trigger_software_command_feature.send_command()

    # Waiting callback
    time.sleep(1)

    # stop acquisition
    cam.stream_off()
    
    print('<Stop acquisition>')

    # Unregister capture callback
    data_stream.unregister_capture_callback()

    # close device
    cam.close_device()


if __name__ == "__main__":
    main()
