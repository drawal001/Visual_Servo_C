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
    std::string root(ROOT);
    cv::Mat image = cv::imread(root + "image/12_2/calibration_1_4.png");
    cv::Mat gary;
    cv::cvtColor(image, gary, cv::COLOR_RGB2GRAY);

    std::vector<cv::Point2f> corner;
    if (!cv::findChessboardCorners(gary, cv::Size(19, 15), corner))
    {
        std::cerr << "Failed to find corners in image" << std::endl;
        return -1;
    }
    const cv::TermCriteria criteria{cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.001};
    cv::cornerSubPix(gary, corner, cv::Size(6, 6), cv::Size(-1, -1), criteria);
    cv::drawChessboardCorners(image, cv::Size(19, 15), corner, true);
    std::cout << corner.size() << std::endl;
    cv::imshow(imagename, image);
    cv::waitKey(0);
    float sum = 0;
    cv::Point2f last_point{};
    for (int i = 0; i < 19; ++i)
    {
        for (int j = 0; j < 15;++j){
            if(j == 0){
                last_point = corner[j * 19 + i];
                continue;
            }
            auto a = sqrt(powf(corner[j * 19 + i].x - last_point.x, 2) + powf(corner[j * 19 + i].y - last_point.y, 2));
            sum += a;
            last_point = corner[j * 19 + i];
        }
    }
    float map_param = 14 * 19 / sum; // map_param = 0.00945188
    std::cout << "map_param: " << map_param << " (mm/pixel)" << std::endl;
    return 0;
}