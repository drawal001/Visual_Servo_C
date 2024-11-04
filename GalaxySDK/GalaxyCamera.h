#pragma once

#include "DxImageProc.h"
#include "GxIAPI.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <opencv2/imgproc.hpp>

    class GxCamera {
public:
    GxCamera(std::string id);
    ~GxCamera();
    const char *getGxError();
    bool init();
    bool reconnect();
    void release();
    bool read(cv::OutputArray image);
    bool retrieve(cv::OutputArray image);

private:
    GX_DEV_HANDLE _handle{};
    std::string _id{};
    bool _init{};
    GX_FRAME_DATA _data{};
    int64_t _payload{};
};