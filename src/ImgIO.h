#ifndef IMGIO_H
#define IMGIO_H

#include <deque>
#include <memory>
#include <thread>


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
    inline const std::shared_ptr<Frame> getFrame();


private:
    void getImgFrmCam();


private:
    std::deque<std::shared_ptr<Frame>>  frameQuque;
    cv::VideoCapture cap;
    std::thread *camThread;
};

#endif // IMGIO_H
