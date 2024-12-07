#include "GalaxyCamera.h"

static std::string window_name = "camera";
static std::string root(ROOT);

void GetPosTemplate(cv::Mat img)
{
    cv::Rect roi(810, 30, 200, 200);
    // cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 4);
    cv::Mat ROI = img(roi).clone();
    cv::imshow(window_name, ROI);
    cv::waitKey(0);
    cv::imwrite(root + "image/12_6/new/PosTemple.png", ROI);
}

void ROI(cv::Mat img){
    cv::Mat image = img.clone();
    cv::Mat temp = cv::imread(root + "image/12_6/new/PosTemple.png");
    cv::Mat result;
    cv::matchTemplate(image, temp, result, cv::TM_SQDIFF_NORMED);
    cv::Point minLoc, maxLoc;
    double minVal, maxVal;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::Point pos = minLoc + cv::Point(-260, 300);
    cv::Rect roi(pos, cv::Size(750, image.size[2] - pos.y));
    // cv::imshow(window_name, image);
    // cv::waitKey(0);
    cv::Point D = minLoc + cv::Point(110, 100 + 1200);
    cv::circle(image, D, 4, cv::Scalar(0, 0, 255), 2);
    cv::rectangle(image, roi, cv::Scalar(0, 0, 255), 4);
    cv::imshow(window_name, image);
    cv::waitKey(0);
}

int main()
{
    

    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, cv::Size(1250, 1000));

    cv::Mat img = cv::imread(root + "image/12_6/topC_1.png");
    // GetPosTemplate(img);
    cv::Mat res;
    ROI(img);
    return 0;
}
