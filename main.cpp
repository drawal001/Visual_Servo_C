#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::cout << "hello" << std::endl;
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera!" << std::endl;
        return -1;
    }
    cv::Mat frame;
    while(true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Could not capture frame!" << std::endl;
            break;
        }
        cv::imshow("frame", frame);

        if (cv::waitKey(30) == 'q') {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();

    return 0;
}