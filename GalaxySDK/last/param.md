图像尺寸
picture_size = (2592, 2048)
jaw_model_size = (700, 800)
jaw_position_points{
    midup = (327.648  45.505)
    center = (329.79, 407.575)
    middown = (331.933 769.645)
}


相机内参
cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 105.9035, 0.04435, 0, 0, 105.689, 0, 0, 0, 1);
cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << 0, 0, 0, 0, 0);

映射关系
map_param = 0.00945188