#ifndef SETTING_H
#define SETTING_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <boost/random.hpp>

#define MAX_VAR  0.25
#define INIT_DEPTH_MEANS 1.0


Eigen::Matrix3f& camera();
cv::Mat& cameraCV();
Eigen::Matrix3f &camInv();
cv::Mat& camInvCV();
Eigen::Matrix<float, 5, 1>& camDistortion();
cv::Mat& camDistortionCV();

float getInitDepth();

float getVal(char* index);
void getConfigueParam(const char *cfgPath = "./Configue.cfg");

#endif // SETTING_H
