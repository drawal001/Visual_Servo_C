#include "GalaxyCamera.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>

constexpr auto CAMERA_TOP = "00-21-49-03-4D-95";

int main()
{
    GxCamera camera1(CAMERA_TOP);
    if (!camera1.IsInit())
    {
        std::cerr << "Failed to init camera1" << std::endl;
        return -1;
    }

    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 105.9035, 0.04435, 0, 0, 105.689, 0, 0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << 0, 0, 0, 0, 0);

    cv::Mat a{};
    std::string window_name = "camera_top";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(1295, 1024));

    cv::Size imageSize(2592, 2048);
    cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0);
    cv::Mat map1, map2;
    cv::Mat R = cv::Mat::eye(3, 3, CV_64F);
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, imageSize, CV_32FC1, map1, map2);
    int count = 0;
    while (camera1.Read(a))
    {

        cv::Mat dst;
        cv::remap(a, dst, map1, map2, cv::INTER_NEAREST);
        cv::imshow(window_name, dst);
        if (cv::waitKey(1) == 27)
        {
            break;
        }
        if (cv::waitKey(1) == 32)
        {

            std::string filename = "E:/WYL_workspace/Visual_Servo_C/GalaxySDK/image/clampddd_" + std::to_string(count) + ".png";
            count++;

            if (!cv::imwrite(filename, dst))
            {
                std::cerr << "Failed to get picture" << std::endl;
                break;
            }
            else
                std::cout << "success to save picture " << count << std::endl;
        }
    }
    cv::destroyAllWindows();
}