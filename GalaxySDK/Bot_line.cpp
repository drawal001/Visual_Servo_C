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
    std::string root = ROOT;
    cv::Mat img = cv::imread(root + "image/11_24/botC_clamp_5.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::Rect roi = cv::Rect(0, 1000, 2592, 1048);
    gray(roi).setTo(cv::Scalar(255));

    cv::imshow(window_name, gray);
    cv::waitKey(0);
    cv::Mat bin;
    cv::threshold(gray, bin, 250, 255, cv::THRESH_BINARY);
    cv::Mat gauss;
    cv::GaussianBlur(bin, gauss, cv::Size(5, 5), 25);
    cv::imshow(window_name, bin);
    cv::waitKey(0);
    cv::imshow(window_name, gauss);
    cv::waitKey(0);

    cv::Mat dst;
    cv::Scharr(gauss, dst, CV_32F, 1, 0);
    cv::Mat edge;
    // cv::Canny(bin, edge, 1, 200, 3, true);
    cv::convertScaleAbs(dst, edge);
    cv::imshow(window_name, edge);
    cv::waitKey(0);

    std::vector<cv::Vec4f> lines;
    cv::HoughLinesP(edge, lines, 1, CV_PI / 180, 250, 1000, 300);
    std::cout << lines.size() << std::endl;
    // 最小二乘拟合
    int n = lines.size() * 2;
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (auto &line : lines)
    {
        // cv::line(img, cv::Point2f(line[0], line[1]), cv::Point2f(line[2], line[3]), cv::Scalar(0, 255, 0), 2);
        // cv::imshow(window_name, img);
        // cv::waitKey(0);
        sum_x += (line[0] + line[2]);
        sum_y += (line[1] + line[3]);
        sum_xy += (line[0] * line[1] + line[2] * line[3]);
        sum_x2 += (line[0] * line[0] + line[2] * line[2]);
    }
    float mean_x = sum_x / n;
    float mean_y = sum_y / n;

    float a = (sum_xy - n * mean_x * mean_y) / (sum_x2 - n * mean_x * mean_x);
    float b = (mean_y - a * mean_x);
    std::cout << "y = " << a << "x + " << b << std::endl;
    cv::line(img, cv::Point2f(200, 200 * a + b), cv::Point2f(2000, 2000 * a + b), cv::Scalar(0, 0, 255), 2);
    cv::imshow(window_name, img);
    cv::waitKey(0);
    // cv::imshow(window_name, img);
    // cv::waitKey(0);
    return 0;
}