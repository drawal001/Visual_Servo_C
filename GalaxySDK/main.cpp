#include "GalaxyCamera.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#define CAMERA_TOP "00-21-49-03-4D-95"

int main()
{

    GxCamera camera1(CAMERA_TOP);
    cv::Mat a{};
    while (true)
    {
        camera1.read(a);
        cv::imshow("camera_top", a);
        if(cv::waitKey(1) == 27)
        {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}