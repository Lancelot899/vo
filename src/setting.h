#ifndef SETTING_H
#define SETTING_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>


inline const Eigen::Matrix3f& camera();
inline const cv::Mat& cameraCV();
inline const Eigen::Matrix<float, 5, 1>& camDistortion();
inline const cv::Mat& camDistortionCV();

inline float getVal(char* index);
void getConfigueParam();

#endif // SETTING_H
