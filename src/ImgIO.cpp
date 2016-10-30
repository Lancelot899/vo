#include "ImgIO.h"
#include "Frame.h"
#include "setting.h"


ImgIO::ImgIO(int width, int height, SrcType srcType, char* )
{
    if(srcType == SRC_VIDEO) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
        cap.open(0);
        assert(cap.isOpened());
        camThread = new std::thread(std::bind(&ImgIO::getImgFrmCam, this));
    }

}

std::shared_ptr<Frame> ImgIO::getFrame()
{
    if(frameQuque.empty())
        return std::shared_ptr<Frame>(new Frame());

    std::shared_ptr<Frame> frame = frameQuque.back();
    frameQuque.pop_back();
    return frame;
}

void ImgIO::getImgFrmCam()
{
    if(frameQuque.size() > 7)
        frameQuque.pop_front();

    cv::Mat mat;
    cap >> mat;
    float exposureTime = cap.get(CV_CAP_PROP_EXPOSURE);

    //! distortion correction
    cv::Mat img;
    cv::undistort(mat, img, cameraCV(), camDistortionCV());

    frameQuque.push_back(std::make_shared<Frame>(img, exposureTime));
}
