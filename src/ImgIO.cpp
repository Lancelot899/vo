#include "ImgIO.h"
#include "Frame.h"


ImgIO::ImgIO(int width, int height, SrcType srcType, char* )
{
    if(srcType == SRC_VIDEO) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }

    camThread = new std::thread(std::bind(&ImgIO::getImgFrmCam, this));
}

void ImgIO::getImgFrmCam()
{
    if(frameQuque.size() > 7)
        frameQuque.pop_front();

    cv::Mat mat;
    float exposureTime = cap.get(CV_CAP_PROP_EXPOSURE);
    cap >> mat;
    frameQuque.push_back(std::shared_ptr<Frame>(new Frame(mat, exposureTime)));
}
