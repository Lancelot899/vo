#include "Frame.h"

Frame::Frame()
{
    id = -1;
    cv::Mat K = cameraCV();
    fx = K.at<float>(0, 0);
    fy = K.at<float>(1, 1);
    cx = K.at<float>(0, 2);
    cy = K.at<float>(1, 2);
    memset(image, 0, sizeof(int) * 5);
    memset(width, 0, sizeof(int) * 5);
    memset(height, 0, sizeof(int) * 5);
    exposureTime = -1.0f;
}

Frame::Frame(int id, cv::Mat img, float exposureTime)
{
    this->id = id;
    cv::Mat K = cameraCV();
    fx = K.at<float>(0, 0);
    fy = K.at<float>(1, 1);
    cx = K.at<float>(0, 2);
    cy = K.at<float>(1, 2);

    width[0] = img.cols;
    height[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height[i] = height[i - 1] / 2;
        width[i] = width[i - 1] / 2;
    }

    this->pose = Sophus::SE3f();
    this->rgbImg = img;
    this->exposureTime = exposureTime;
}

Frame::Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose)
{
    this->id = id;
    cv::Mat K = cameraCV();
    fx = K.at<float>(0, 0);
    fy = K.at<float>(1, 1);
    cx = K.at<float>(0, 2);
    cy = K.at<float>(1, 2);

    width[0] = img.cols;
    height[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height[i] = height[i - 1] / 2;
        width[i] = width[i - 1] / 2;
    }

    this->pose = pose;
    this->rgbImg = img;
    this->exposureTime = exposureTime;
}

Frame::Frame(int id, cv::Mat img, float exposureTime, std::map<int, voPoint> obsPoints)
{
    this->id = id;
    cv::Mat K = cameraCV();
    fx = K.at<float>(0, 0);
    fy = K.at<float>(1, 1);
    cx = K.at<float>(0, 2);
    cy = K.at<float>(1, 2);

    width[0] = img.cols;
    height[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height[i] = height[i - 1] / 2;
        width[i] = width[i - 1] / 2;
    }

    this->pose = Sophus::SE3f();
    this->rgbImg = img;
    this->exposureTime = exposureTime;
    this->obsPoints.swap(obsPoints);
}

Frame::Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose, std::map<int, voPoint> obsPoints)
{
    this->id = id;
    cv::Mat K = cameraCV();
    fx = K.at<float>(0, 0);
    fy = K.at<float>(1, 1);
    cx = K.at<float>(0, 2);
    cy = K.at<float>(1, 2);

    width[0] = img.cols;
    height[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height[i] = height[i - 1] / 2;
        width[i] = width[i - 1] / 2;
    }

    this->pose = pose;
    this->rgbImg = img;
    this->exposureTime = exposureTime;
    this->obsPoints.swap(obsPoints);
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
