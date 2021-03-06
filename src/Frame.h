#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <vector>
#include <atomic>
#include <map>

#include <opencv2/opencv.hpp>
#include <sophus/se3.hpp>

#include "src/setting.h"

#define IMGINDEX(u, v) (u * height_[4] + v)

class Frame;

struct voPoint {
    voPoint(int id, int u, int v) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        depth = getInitDepth();
        isDepthBusy = false;
    }

    voPoint(int id, int u, int v, float depth) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        this->depth = depth;
        isDepthBusy = false;
    }

    voPoint(int id, int u, int v, float depth, std::vector< std::shared_ptr<Frame> >& obsFrame, std::vector< cv::Point2i > & obsCoords) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        this->depth = depth;
        this->obsFrame.assign(obsFrame.begin(), obsFrame.end());
        this->obsCoords.assign(obsCoords.begin(), obsCoords.end());
        isDepthBusy = false;
    }

    voPoint(const voPoint& oth) {
        frameID = oth.frameID;
        u = oth.u;
        v = oth.v;
        depth = oth.depth;
        this->obsFrame.assign(oth.obsFrame.begin(), oth.obsFrame.end());
        this->obsCoords.assign(oth.obsCoords.begin(), oth.obsCoords.end());
        this->neighber.assign(oth.neighber.begin(), oth.neighber.end());
        isDepthBusy = false;
    }

    int frameID;
    int u, v;
    std::atomic_bool isDepthBusy;
    float depth;
    std::vector< std::shared_ptr<Frame> > obsFrame;
    std::vector< cv::Point2i > obsCoords;
    std::vector< cv::Point2i > neighber;
};


class Frame
{
public:
    Frame();
    Frame(int id, cv::Mat img, float exposureTime);
    Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f& pose);
    Frame(int id, cv::Mat img, float exposureTime, std::map<int, voPoint> obsPoints);
    Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose, std::map<int, voPoint> obsPoints);

    const cv::Mat &RGBImg();
    bool isEmpty();

    const std::map<int, voPoint>& obsPoints();
    void  setDepth(int u, int v, float val);
    void  setPose(Sophus::SE3f &pose);
    float getDepth(int u, int v);
     int  getID() { return id; }
    float fx() { return fx_; }
    float fy() { return fy_; }
    float cx() { return cx_; }
    float cy() { return cy_; }
     int  height() { return height_[4]; }
     int  width() { return width_[4]; }
    const Sophus::SE3f &pose();
    float exposureTime();
    const float *Image(int i = 0);
    const Eigen::Vector3f* gradients(int i = 0);
    const float* maxGradients(int i = 0);
    float a() {return a_; }
    float b() {return b_; }
    void setA(float a) {a_ = a;}
    void setB(float b) {b_ = b;}

private:
    int id;
    std::atomic<float> a_, b_;
    float fx_, fy_, cx_, cy_;
    cv::Mat rgbImg;
    float *image[5];
    Eigen::Vector3f *gradients_[5];
    float *maxGradients_[5];
    int width_[5];
    int height_[5];
    float exposureTime_;
    std::atomic_bool isPoseBusy;
    Sophus::SE3f pose_;
    std::map<int, voPoint> obsPoints_;
};

#endif // FRAME_H
