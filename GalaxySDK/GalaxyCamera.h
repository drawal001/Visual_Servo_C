#pragma once

#include "DxImageProc.h"
#include "GxIAPI.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class GxCamera
{
public:
    GxCamera(std::string_view id);
    ~GxCamera();
    const char *GetGxError();
    bool Init();
    bool IsInit();
    bool Reconnect();
    void Release();
    bool Read(cv::OutputArray image);
    bool Retrieve(cv::OutputArray image);

private:
    GX_DEV_HANDLE _handle{};
    std::string _id{};
    bool _isInit{};
    GX_FRAME_DATA _data{};
    int64_t _payload{};
};
