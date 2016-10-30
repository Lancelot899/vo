#ifndef SETTING_H
#define SETTING_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>


Eigen::Matrix3f& camera();
cv::Mat& cameraCV();
Eigen::Matrix<float, 5, 1>& camDistortion();
cv::Mat& camDistortionCV();

float getVal(char* index);
void getConfigueParam(const char *cfgPath = "./Configue.cfg");

#endif // SETTING_H
