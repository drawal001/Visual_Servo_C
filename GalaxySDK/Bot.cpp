#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int main()
{

    std::string window_name = "camera";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(1295, 1024));

    std::string root(ROOT);
    // 初始化模板
    cv::Mat model = cv::imread(root + "image/12_1/botmodel_clamp.png", 0);
    std::vector<cv::Point2f> _points;
    _points.push_back(cv::Point2f(48.6546, 135.857));
    _points.push_back(cv::Point2f(49.0382, 83.816));
    _points.push_back(cv::Point2f(482.029, 87.0076));
    _points.push_back(cv::Point2f(481.646, 139.049));
    cv::Mat img = cv::imread(root + "image/11_24/botC_clamp_7.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    float a = -0.00201866, b = 787.792;
    cv::line(img, cv::Point2f(200, 200 * a + b), cv::Point2f(2000, 2000 * a + b), cv::Scalar(0, 0, 255), 2);
    cv::imshow(window_name, img);
    cv::waitKey(0);

    // 模板匹配
    cv::Mat res;
    cv::matchTemplate(gray, model, res, cv::TM_SQDIFF_NORMED);
    cv::Point minLoc, maxLoc;
    double minVal, maxVal;
    cv::minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::Point2f minLoc_(minLoc.x, minLoc.y);
    for (int i = 0; i < 4; ++i)
    {
        cv::line(img, _points[i] + minLoc_, _points[(i + 1) % 4] + minLoc_, cv::Scalar(0, 0, 255), 2);
    }
    cv::imshow(window_name, img);
    cv::waitKey(0);
    double distance = 0;
    for (int i = 1; i < 3; ++i)
    {
        distance += (abs(a * (_points[i].x + minLoc_.x) - _points[i].y - minLoc_.y + b) / sqrt(a * a + 1));
    }
    distance /= 2.0;
    std::cout << distance << std::endl;
    cv::Point2f bias(0, distance);
    for (int i = 0; i < 4; ++i)
    {
        cv::line(img, _points[i] + minLoc_ + bias, _points[(i + 1) % 4] + minLoc_ + bias, cv::Scalar(0, 0, 255), 2);
    }
    cv::imshow(window_name, img);
    cv::waitKey(0);
    return 0;
}