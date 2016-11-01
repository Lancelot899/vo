#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>
#include <sophus/se3.hpp>

#include "src/setting.h"

class Frame;

struct voPoint {
    voPoint() {
        depth = getInitDepth();
    }

    voPoint(float depth) {
        this->depth = depth;
    }

    voPoint(float depth, std::vector< std::shared_ptr<Frame> >& obsFrame, std::vector< cv::Point2i > & obsCoords) {
        this->depth = depth;
        this->obsFrame.assign(obsFrame.begin(), obsFrame.end());
        this->obsCoords.assign(obsCoords.begin(), obsCoords.end());
    }

    voPoint(const voPoint& oth) {
        depth = oth.depth;
        this->obsFrame.assign(oth.obsFrame.begin(), oth.obsFrame.end());
        this->obsCoords.assign(oth.obsCoords.begin(), oth.obsCoords.end());
    }

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
    Frame(int id, cv::Mat img, float exposureTime, std::vector<voPoint>& obsPoints);
    Frame(int id, cv::Mat img, float exposureTime, Sophus::SE3f &pose, std::vector<voPoint>& obsPoints);

    inline const float *getImage(int i);
    const cv::Mat &RGBImg();
    bool isEmpty();
    int getID() { return id; }

private:
    int id;
    cv::Mat rgbImg;
    float *image[5];
    Eigen::Vector3f *gradients[5];
    float *maxGradient[5];
    int width[5];
    int height[5];
    float exposureTime;
    Sophus::SE3f pose;
    std::vector<voPoint> obsPoints;
};

#endif // FRAME_H
