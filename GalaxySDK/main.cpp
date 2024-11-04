#include "GalaxyCamera.h"

#include <iostream>
#include <opencv2/highgui.hpp>

constexpr auto CAMERA_TOP = "00-21-49-03-4D-95";

int main()
{
    GxCamera camera1(CAMERA_TOP);
    // 加个 isOpened() 判断

    cv::Mat a{};
    std::string window_name = "camera_top";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(640, 480));
    while (camera1.read(a))
    {
        cv::imshow(window_name, a);
        if (cv::waitKey(1) == 27)
        {
            break;
        }
    }
}