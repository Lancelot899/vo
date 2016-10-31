#ifndef VOSYSTEM_H
#define VOSYSTEM_H

#include <vector>
#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <atomic>
#include <opencv2/opencv.hpp>

#include <Eigen/Dense>

class ImgIO;
class Frame;
class Optimizater;

class voSystem {
public:
    voSystem();
    bool running();

    std::shared_ptr<Frame>& getCurrentFrame();
    const cv::Mat &getImage(void);
    int getPoints(std::shared_ptr<std::vector<std::shared_ptr<Eigen::Vector3f>>> &points);

    bool queryUpdate()       { return sysNeedUpdate; }
    void resetSysUpdate()    { sysNeedUpdate = false; }
private:
    void tracking();
    void optimize();

private:
    std::shared_ptr<ImgIO>         imgIO;
    std::shared_ptr<Optimizater>   optimizer;

    boost::thread          *trackThread;
    boost::thread          *optThread;

    std::atomic<std::shared_ptr<Frame>> currentFrame;

    boost::shared_mutex    keyFramesMutex;
    std::shared_ptr<Frame> keyFrames;
};

#endif // VOSYSTEM_H
