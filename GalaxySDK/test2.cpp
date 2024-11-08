#include "GalaxyCamera.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

// #include <filesystem>

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

        std::vector<cv::Point2f> corner;
        if (!cv::findChessboardCorners(dst, cv::Size(19, 15), corner))
        {
            std::cerr << "Failed to find corners in image" << std::endl;
            continue;
        }
        const cv::TermCriteria criteria{cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.001};
        cv::cornerSubPix(dst, corner, cv::Size(6, 6), cv::Size(-1, -1), criteria);
        cv::drawChessboardCorners(dst, cv::Size(19, 15), corner, true);

        cv::imshow(window_name, dst);
        if (cv::waitKey(1) == 27)
        {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}