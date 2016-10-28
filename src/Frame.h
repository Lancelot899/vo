#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>


class Frame
{
public:
    Frame(cv::Mat img);

private:
    cv::Mat mat;
    float *image[5];
    int width;
    int height;
};

#endif // FRAME_H
