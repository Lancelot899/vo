#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>


class Frame
{
public:
    Frame(cv::Mat img, float exposureTime);
    const float *getImage(int i);
    const cv::Mat &RGBImg();

private:
    cv::Mat mat;
    float *image[5];
    int width;
    int height;
    float exposureTime;
};

#endif // FRAME_H
