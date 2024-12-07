#include "GalaxyCamera.h"

int main()
{
    // 导入图像 -> 中值滤波去噪点
    cv::Mat img = cv::imread("../clamp.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::Mat Imgblur;

    cv::medianBlur(gray, Imgblur, 5);
    // 左右分批处理
    cv::Rect roi_left = cv::Rect(0, 0, 400, 450);
    cv::Mat imgRoi_left = Imgblur(roi_left).clone();
    cv::Rect roi_right = cv::Rect(400, 0, 450, 450);
    cv::Mat imgRoi_right = Imgblur(roi_right).clone();
    // 提取轮廓
    cv::Mat edges_left;
    cv::Canny(imgRoi_left, edges_left, 100, 150);
    std::vector<std::vector<cv::Point>> contours_left;
    cv::findContours(edges_left, contours_left, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> contour_left;
    for (auto &contour : contours_left)
    {
        if (contour.size() > 50)
        {
            contour_left.insert(contour_left.end(), contour.begin(), contour.end());
        }
    }

    cv::Mat edges_right;
    cv::Canny(imgRoi_right, edges_right, 100, 150);
    std::vector<std::vector<cv::Point>> contours_right;
    cv::findContours(edges_right, contours_right, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> contour_right;
    for (auto &contour : contours_right)
    {
        if (contour.size() > 50)
        {
            contour_right.insert(contour_right.end(), contour.begin(), contour.end());
        }
    }

    // std::cout << contour_right.size() << std::endl;

    cv::Mat black_left = cv::Mat(imgRoi_left.size(), imgRoi_left.type(), cv::Scalar::all(0));
    cv::Mat black_right = cv::Mat(imgRoi_right.size(), imgRoi_right.type(), cv::Scalar::all(0));

    // 凸包
    std::vector<cv::Point> hull_left;
    cv::convexHull(cv::Mat(contour_left), hull_left);
    std::vector<cv::Point> hull_right;
    cv::convexHull(cv::Mat(contour_right), hull_right);

    // 绘制
    if (hull_left.size() > 1)
    {
        for (int i = 0; i < hull_left.size() - 1; i++)
        {
            cv::line(black_left, hull_left[i], hull_left[i + 1], cv::Scalar(255), 2);
        }
        cv::line(black_left, hull_left[hull_left.size() - 1], hull_left[0], cv::Scalar(255), 2);
    }

    if (hull_right.size() > 1)
    {
        for (int i = 0; i < hull_right.size() - 1; i++)
        {
            cv::line(black_right, hull_right[i], hull_right[i + 1], cv::Scalar(255), 2);
        }
        cv::line(black_right, hull_right[hull_right.size() - 1], hull_right[0], cv::Scalar(255), 2);
    }

    // 霍夫直线拟合
    std::vector<cv::Vec4i> lines_left;
    cv::HoughLinesP(black_left, lines_left, 1, CV_PI / 180, 50, 400, 20);
    // std::cout << lines_left.size() << std::endl;
    std::vector<cv::Vec4i> lines_right;
    cv::HoughLinesP(black_right, lines_right, 1, CV_PI / 180, 50, 375, 20);
    std::cout << lines_right.size() << std::endl;

    // 绘制直线
    for (auto &line : lines_left)
    {
        cv::line(img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 2);
        float angle_left = atan2f((line[3] - line[1]), (line[2] - line[0])) * (-180) / CV_PI;
        cv::putText(img, std::to_string(angle_left), cv::Point(line[2], line[3]), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
    }
    for (auto &line : lines_right)
    {
        cv::line(img, cv::Point(line[0], line[1]) + cv::Point(400, 0), cv::Point(line[2] - 5, line[3]) + cv::Point(400, 0), cv::Scalar(0, 0, 255), 2);
        float angle_right = atan2f((line[3] - line[1]), (line[2] - line[0])) * (-180) / CV_PI;
        cv::putText(img, std::to_string(angle_right), cv::Point(line[2], line[3]) + cv::Point(400, 0), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
    }
    cv::Point2f up = 0.5 * (cv::Point(lines_right[0][2] - 5, lines_right[0][3]) + cv::Point(lines_left[0][2], lines_right[0][3]) + cv::Point(400, 0));
    cv::Point2f down = 0.5 * (cv::Point(lines_right[0][0], lines_right[0][1]) + cv::Point(lines_left[0][0], lines_right[0][1]) + cv::Point(400, 0));
    down = up - 0.1 * (up - down);
    cv::line(img, up, down, cv::Scalar(0, 0, 255), 2);
    float angle = atan2f((up.y - down.y), (up.x - down.x)) * (-180) / CV_PI;
    cv::putText(img, std::to_string(angle), up, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

    std::cout << up << std::endl;
    std::cout << down << std::endl;

    cv::imshow("dsdfs", edges_left);
    cv::waitKey(0);
    cv::imshow("dsdfs", edges_right);
    cv::waitKey(0);
    cv::imshow("dsdfs", black_left);
    cv::waitKey(0);
    cv::imshow("dsdfs", black_right);
    cv::waitKey(0);

    cv::imshow("dsdfs", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}