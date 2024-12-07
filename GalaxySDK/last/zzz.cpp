#include "GalaxyCamera.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    std::string root(ROOT);
    cv::Mat image = cv::imread(root + "image/12_5/top_1.png");

    //感兴趣区域
    cv::Rect roi(450, 700, 750, 1450);
    cv::rectangle(image, roi, cv::Scalar(0, 0, 255), 2);
    // cv::Mat roiImg = image(roi);

    // cv::Mat img = cv::Mat(image.size(), image.type(), cv::Scalar::all(255));
    // roiImg.copyTo(img(roi));



    std::string windowname2 = "Binary";
    cv::namedWindow(windowname2, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowname2, cv::Size(1295, 1024));
    cv::imshow(windowname2, image);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}