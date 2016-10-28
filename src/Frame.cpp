#include "Frame.h"

Frame::Frame(cv::Mat img, float exposureTime)
{
    width = img.cols;
    height = img.rows;
    this->mat = img;
    this->exposureTime = exposureTime;
}

const float* Frame::getImage(int i)
{
    if(i >= 5) return nullptr;
    return image[i];
}

inline const cv::Mat &Frame::RGBImg() {
    return mat;
}
