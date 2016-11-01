#include <thread>

#include "Frame.h"

Frame::Frame()
{
    id = -1;
    cv::Mat K = cameraCV();
    fx_ = K.at<float>(0, 0);
    fy_ = K.at<float>(1, 1);
    cx_ = K.at<float>(0, 2);
    cy_ = K.at<float>(1, 2);
    memset(image, 0, sizeof(int) * 5);
    memset(width_, 0, sizeof(int) * 5);
    memset(height_, 0, sizeof(int) * 5);
    exposureTime_ = -1.0f;
    isPoseBusy = false;
}

Frame::Frame(int id, cv::Mat img, float exposureTime)
{
    isPoseBusy = false;
    this->id = id;
    cv::Mat K = cameraCV();
    fx_ = K.at<float>(0, 0);
    fy_ = K.at<float>(1, 1);
    cx_ = K.at<float>(0, 2);
    cy_ = K.at<float>(1, 2);

    width_[0] = img.cols;
    height_[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height_[i] = height_[i - 1] >> 1;
        width_[i] = width_[i - 1] >> 1;
    }

    this->pose_ = Sophus::SE3f();
    this->rgbImg = img;
    this->exposureTime_ = exposureTime;
}

Frame::Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose)
{
    isPoseBusy = false;
    this->id = id;
    cv::Mat K = cameraCV();
    fx_ = K.at<float>(0, 0);
    fy_ = K.at<float>(1, 1);
    cx_ = K.at<float>(0, 2);
    cy_ = K.at<float>(1, 2);

    width_[0] = img.cols;
    height_[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height_[i] = height_[i - 1] >> 1;
        width_[i] = width_[i - 1] >> 1;
    }

    this->pose_ = pose;
    this->rgbImg = img;
    this->exposureTime_ = exposureTime;
}

Frame::Frame(int id, cv::Mat img, float exposureTime, std::map<int, voPoint> obsPoints)
{
    isPoseBusy = false;
    this->id = id;
    cv::Mat K = cameraCV();
    fx_ = K.at<float>(0, 0);
    fy_ = K.at<float>(1, 1);
    cx_ = K.at<float>(0, 2);
    cy_ = K.at<float>(1, 2);

    width_[0] = img.cols;
    height_[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height_[i] = height_[i - 1] >> 1;
        width_[i] = width_[i - 1] >> 1;
    }

    this->pose_ = Sophus::SE3f();
    this->rgbImg = img;
    this->exposureTime_ = exposureTime;
    this->obsPoints_.swap(obsPoints);
}

Frame::Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose, std::map<int, voPoint> obsPoints)
{
    isPoseBusy = false;
    this->id = id;
    cv::Mat K = cameraCV();
    fx_ = K.at<float>(0, 0);
    fy_ = K.at<float>(1, 1);
    cx_ = K.at<float>(0, 2);
    cy_ = K.at<float>(1, 2);

    width_[0] = img.cols;
    height_[0] = img.rows;
    for(int i = 1; i < 5; ++i) {
        height_[i] = height_[i - 1] >> 1;
        width_[i] = width_[i - 1] >> 1;
    }

    this->pose_ = pose;
    this->rgbImg = img;
    this->exposureTime_ = exposureTime;
    this->obsPoints_.swap(obsPoints);
}

const float* Frame::Image(int i)
{
    if(i >= 5) return nullptr;
    return image[i];
}

const Eigen::Vector3f *Frame::gradients(int i) {
    if (i >= 5 && i < 0) return nullptr;
    return gradients_[i];
}

const float *Frame::maxGradients(int i) {
    if (i >= 5 && i < 0) return nullptr;
    return maxGradients_[i];
}

const cv::Mat &Frame::RGBImg() {
    return rgbImg;
}

bool Frame::isEmpty() {
    return rgbImg.empty();
}

const std::map<int, voPoint> &Frame::obsPoints() {return obsPoints_;}

void Frame::setDepth(int u, int v, float val) {
    auto it = obsPoints_.find(u * height_[4] + v);
    if(it != obsPoints_.end()) {
        while((it->second).isDepthBusy == true) std::this_thread::yield();
        (it->second).isDepthBusy = true;
        (it->second).depth = val;
        (it->second).isDepthBusy = false;
    }
}

void Frame::setPose(Sophus::SE3f &pose) {
    while(isPoseBusy == true) std::this_thread::yield();
    isPoseBusy = true;
    this->pose_ = pose;
    isPoseBusy = false;
}

float Frame::getDepth(int u, int v) {
    auto it = obsPoints_.find(u * height_[4] + v);
    if(it != obsPoints_.end()) {
        while((it->second).isDepthBusy == true) std::this_thread::yield();
        return (it->second).depth;
    }

    return -1.0f;
}

const Sophus::SE3f &Frame::pose() {
    while(isPoseBusy == true) std::this_thread::yield();
    return pose_;
}

float Frame::exposureTime() {
    return exposureTime_;
}
