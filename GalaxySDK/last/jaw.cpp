#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int main()
{
    std::string root(ROOT);
    cv::Mat img = cv::imread(root + "image/12_2/jaw_model.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::Mat bin;
    cv::threshold(gray, bin, 60, 255, cv::THRESH_BINARY);
    cv::Rect roi(220, 60, 200, 80);
    cv::Mat black = cv::Mat(img.size(), gray.type(), cv::Scalar::all(0));
    bin(roi).copyTo(black(roi));

    cv::Mat inv_bin;
    cv::bitwise_not(black, inv_bin);

    cv::Mat edge;
    cv::Canny(inv_bin, edge, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edge, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> contours_;
    for (auto &contour : contours)
    {
        contours_.insert(contours_.end(), contour.begin(), contour.end());
    }
    cv::Point2f center;
    float r;
    cv::minEnclosingCircle(contours_, center, r);
    return 0;
}