#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>
#include <sophus/se3.hpp>
#include <map>

#include "src/setting.h"

class Frame;

struct voPoint {
    voPoint(int id, int u, int v) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        depth = getInitDepth();
    }

    voPoint(int id, int u, int v, float depth) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        this->depth = depth;
    }

    voPoint(int id, int u, int v, float depth, std::vector< std::shared_ptr<Frame> >& obsFrame, std::vector< cv::Point2i > & obsCoords) {
        this->frameID = id;
        this->u = u;
        this->v = v;
        this->depth = depth;
        this->obsFrame.assign(obsFrame.begin(), obsFrame.end());
        this->obsCoords.assign(obsCoords.begin(), obsCoords.end());
    }

    voPoint(const voPoint& oth) {
        frameID = oth.frameID;
        u = oth.u;
        v = oth.v;
        depth = oth.depth;
        this->obsFrame.assign(oth.obsFrame.begin(), oth.obsFrame.end());
        this->obsCoords.assign(oth.obsCoords.begin(), oth.obsCoords.end());
    }
    int frameID;
    int u, v;
    float depth;
    std::vector< std::shared_ptr<Frame> > obsFrame;
    std::vector< cv::Point2i > obsCoords;
};


class Frame
{
public:
    Frame();
    Frame(int id, cv::Mat img, float exposureTime);
    Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f& pose);
    Frame(int id, cv::Mat img, float exposureTime, std::map<int, voPoint> obsPoints);
    Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose, std::map<int, voPoint> obsPoints);

    inline const float *getImage(int i);
    const cv::Mat &RGBImg();
    bool isEmpty();
    int getID() { return id; }
    const std::map<int, voPoint>& getObsPoints() {return obsPoints;}
    void setDepth(int u, int v, float val) {
        auto it = obsPoints.find(u * height[4] + v);
        if(it != obsPoints.end())
            (it->second).depth = val;
    }

    float getDepth(int u, int v) {
        auto it = obsPoints.find(u * height[4] + v);
        if(it != obsPoints.end())
            return (it->second).depth;

        return -1.0f;
    }

private:
    int id;
    float fx, fy, cx, cy;
    cv::Mat rgbImg;
    float *image[5];
    Eigen::Vector3f *gradients[5];
    float *maxGradient[5];
    int width[5];
    int height[5];
    float exposureTime;
    Sophus::SE3f pose;
    std::map<int, voPoint> obsPoints;
};

#endif // FRAME_H
