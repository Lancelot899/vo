#include "Frame.h"

Frame::Frame()
{
    memset(image, 0, sizeof(int) * 5);
    memset(width, 0, sizeof(int) * 5);
    memset(height, 0, sizeof(int) * 5);
    exposureTime = -1.0f;
}

Frame::Frame(cv::Mat img, float exposureTime)
{
    width[0] = img.cols;
    height[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height[i] = height[i - 1] / 2;
        width[i] = width[i - 1] / 2;
    }

    this->rgbImg = img;
    this->exposureTime = exposureTime;
}

const float* Frame::getImage(int i)
{
    if(i >= 5) return nullptr;
    return image[i];
}

const cv::Mat &Frame::RGBImg() {
    return rgbImg;
}

bool Frame::isEmpty() {
    return rgbImg.empty();
}
