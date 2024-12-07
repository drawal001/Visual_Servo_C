#include "GalaxyCamera.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    cv::Mat image = cv::imread("../image/clampddd_3.png");
    if (image.empty())
    {
        std::cerr << "Failed to load picture" << std::endl;
        return -1;
    }
    // 感兴趣区域
    cv::Rect roi(0, 430, 2592, 750);
    cv::Mat roiImg = image(roi);

    cv::Mat img = cv::Mat(image.size(), image.type(), cv::Scalar::all(255));
    roiImg.copyTo(img(roi));

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);

    cv::Mat image_binary;
    cv::threshold(gray, image_binary, 101, 255, cv::THRESH_BINARY);
    cv::Mat image_Gauss;
    cv::GaussianBlur(image_binary, image_Gauss, cv::Size(7, 7), 0, 0, cv::BORDER_DEFAULT);
    // Canny边缘检测
    cv::Mat edges;
    cv::Canny(image_Gauss, edges, 50, 150);
    // 轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //压缩
    std::vector<cv::Point> contours_1;
    for(const auto& contour:contours) {
        //丢弃小的轮廓
        if(contour.size()<200){
            continue;
        }
        contours_1.insert(contours_1.end(), contour.begin(), contour.end());
    }

    // 凸包
    std::vector<cv::Point> hull;
    cv::convexHull(cv::Mat(contours_1), hull);

    //绘制
    cv::Mat hullImg = cv::Mat::zeros(image.size(), gray.type());
    if (hull.size() > 1)
    {
        for (int i = 0; i < hull.size() - 1; i++)
        {
            cv::line(hullImg, hull[i], hull[i + 1], cv::Scalar(255), 2);
        }
        cv::line(hullImg, hull[hull.size() - 1], hull[0], cv::Scalar(255), 2);
    }
    //获得最小外接矩形
    cv::RotatedRect rect = cv::minAreaRect(hull);
    cv::Point2f rectPoints[4];
    rect.points(rectPoints);

    // 绘制最小外接矩形
    for (int i = 0; i < 4; i++)
    {
        line(hullImg, rectPoints[i], rectPoints[(i + 1)%4], cv::Scalar(255), 2);
    }
    //绘制中轴线
    int shortindex = (rect.size.width < rect.size.height) ? 3 : 2;
    cv::Point2f midPoint = 0.5 * (rectPoints[shortindex] + rectPoints[(shortindex + 1)%4]);
    cv::line(image, rect.center, midPoint, cv::Scalar(0,0,255), 2);


    std::string windowname2 = "Binary";
    cv::namedWindow(windowname2, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowname2, cv::Size(1295, 1024));
    cv::imshow(windowname2, image);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}