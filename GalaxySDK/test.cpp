#include "GalaxyCamera.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

// #include <filesystem>

int main()
{
    std::string imagename = "Calibration_board";
    cv::namedWindow(imagename, cv::WINDOW_NORMAL);
    cv::resizeWindow(imagename, cv::Size(1295, 1024));

    std::string filename = "../image/calibration_4.png";
    cv::Mat image = cv::imread(filename);
    if (image.empty())
    {
        std::cerr << "Failed to read image" << std::endl;
        return -1;
    }
    cv::imshow(imagename, image);
    // if(cv::waitKey(0) == 32){
    //     std::cout << "good" << std::endl;
    // }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::imshow(imagename, gray);
    // if (cv::waitKey(0) == 32)
    // {
    //     std::cout << "good" << std::endl;
    // }
    std::vector<cv::Point2f> corner;
    if (!cv::findChessboardCorners(gray, cv::Size(19, 15), corner))
    {
        std::cerr << "Failed to find corners in image" << std::endl;
        return -1;
    }
    const cv::TermCriteria criteria{cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.001};
    cv::cornerSubPix(gray, corner, cv::Size(6, 6), cv::Size(-1, -1), criteria);
    cv::drawChessboardCorners(image, cv::Size(19, 15), corner, true);

    cv::imshow(imagename, image);
    cv::waitKey(0);
    return 0;
}