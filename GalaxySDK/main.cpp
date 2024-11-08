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

    cv::Mat cameraMatrix_L = (cv::Mat_<double>(3, 3) << 105.843, 0.0543, 0, 0, 105.641, 0, 0, 0, 1);
    cv::Mat cameraMatrix_R = (cv::Mat_<double>(3, 3) << 105.964, 0.0344, 0, 0, 105.737, 0, 0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << 0, 0, 0, 0, 0);

    cv::Mat a{};
    std::string window_name = "camera_top";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(640, 480));

    cv::Size imageSize(640,480);
    cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(cameraMatrix_L, distCoeffs, imageSize, 0);
    cv::Mat map1, map2;
    cv::Mat R = cv::Mat::eye(3, 3, CV_64F);
    cv::initUndistortRectifyMap(cameraMatrix_L, distCoeffs, R, newCameraMatrix, imageSize, CV_32FC1, map1, map2);
    int count = 0;
    while (camera1.Read(a))
    {
        int width = a.cols;
        int height = a.rows;

        cv::Mat dst;
        cv::remap(a, dst, map1, map2, cv::INTER_NEAREST);
        cv::imshow(window_name, a);
        if (cv::waitKey(1) == 27)
        {
            std::cout << width << "  " << height << std::endl;
            break;
        }
        if (cv::waitKey(1) == 32){
            std::string filename = "image/clamp_" + std::to_string(count) + ".png";
            count++;
            cv::imwrite(filename, dst);
        }
    }
    cv::destroyAllWindows();
}