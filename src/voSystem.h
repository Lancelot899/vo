#ifndef VOSYSTEM_H
#define VOSYSTEM_H

#include <deque>
#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

class ImgIO;
class Frame;

class voSystem {
public:
    voSystem();
    bool running();

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
