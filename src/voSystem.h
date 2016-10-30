#ifndef VOSYSTEM_H
#define VOSYSTEM_H

#include <vector>
#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <opencv2/opencv.hpp>

#include <Eigen/Dense>

class ImgIO;
class Frame;

class voSystem {
public:
    voSystem();
    bool running();


    int getImage(cv::Mat &input);
    int getPoints(std::shared_ptr<std::vector<std::shared_ptr<Eigen::Vector3f>>> &points);

private:
    void tracking();
    void optimize();

private:
    std::shared_ptr<ImgIO> imgIO;
    boost::thread          *trackThread;
    boost::thread          *optThread;

    boost::shared_mutex    currentFrameMutex;
    std::shared_ptr<Frame> currentFrame;

    boost::shared_mutex    keyFramesMutex;
    std::shared_ptr<Frame> keyFrames;
};

#endif // VOSYSTEM_H
