#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int main()
{
    // 模板
    std::string window_name = "camera";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(1400, 500));
    std::string root = ROOT;
    // cv::Mat img = cv::imread(root + "image/11_24/botC_clamp_4.png");
    // cv::Rect roi(1100, 350, 700, 250);
    // // cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 2);
    // cv::Mat ROI = img(roi).clone();
    // cv::imwrite(root + "image/12_1/botmodel_clamp.png", ROI);

    cv::Mat img = cv::imread(root + "image/12_1/botmodel_clamp.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    // cv::circle(img, cv::Point2f(75, 85), 2, cv::Scalar(0, 0, 255));
    // cv::circle(img, cv::Point2f(455, 86), 2, cv::Scalar(0, 0, 255));
    // cv::circle(img, cv::Point2f(465, 86), 2, cv::Scalar(0, 0, 255));
    // cv::circle(img, cv::Point2f(475, 86), 2, cv::Scalar(0, 0, 255));
    cv::Mat bin;
    cv::threshold(gray, bin, 200, 255, cv::THRESH_BINARY);
    cv::Mat dst_1, dst_2;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(bin, dst_1, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(dst_1, dst_2, cv::MORPH_CLOSE, kernel);

    cv::imshow(window_name, dst_2);
    cv::waitKey(0);

    cv::Mat edge;
    cv::Canny(dst_2, edge, 50, 150);
    cv::imshow(window_name, edge);
    cv::waitKey(0);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edge, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> contours_1;
    for (auto &contour : contours)
    {
        if (contour.size() < 20)
        {
            continue;
        }
        contours_1.insert(contours_1.end(), contour.begin(), contour.end());
    }
    std::cout << contours_1.size() << std::endl;

    cv::RotatedRect rect = cv::minAreaRect(contours_1);
    cv::Point2f rectPoints[4];
    rect.points(rectPoints);

    // 绘制最小外接矩形
    for (int i = 0; i < 4; i++)
    {
        cv::line(img, rectPoints[i], rectPoints[(i + 1) % 4], cv::Scalar(255), 2);
        std::cout << rectPoints[i] << std ::endl;
    }
    cv::imshow(window_name, img);
    cv::waitKey(0);

    // _points.push_back(cv::Point2f(48.6546, 135.857));
    // _points.push_back(cv::Point2f(49.0382, 83.816));
    // _points.push_back(cv::Point2f(482.029, 87.0076));
    // _points.push_back(cv::Point2f(481.646, 139.049));
    // //
    // float a = -0.00201866, b = 787.792;
    // cv::line(img, cv::Point2f(200, 200 * a + b), cv::Point2f(2000, 2000 * a + b), cv::Scalar(0, 0, 255), 2);

    return 0;
}