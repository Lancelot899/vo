#ifndef IMGIO_H
#define IMGIO_H

#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
//#include <boost/thread/condition_variable.hpp>

#include <opencv2/opencv.hpp>

class Frame;

class ImgIO
{
public:
    enum {
        CacheMaxSize = 5
    };

    enum SrcType {
        SRC_IMGS,
        SRC_VIDEO
    };

    ImgIO(int width = 640, int height = 480, SrcType srcType = SRC_VIDEO, char* imgSrc = nullptr /* img dir */);
    std::shared_ptr<Frame> &getFrame();

    size_t getDequeSize(){
        return frameQuque.size();
    }

private:
    void getImgFrmCam();


private:
    boost::shared_mutex                 queueMutex;
    std::deque<std::shared_ptr<Frame>>  frameQuque;
    std::condition_variable             conditionVar;

    std::mutex              curFrameMutex;
    std::shared_ptr<Frame>  curFrame;

    cv::VideoCapture cap;
    std::thread *camThread;
};

#endif // IMGIO_H
