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

std::shared_ptr<Frame> &ImgIO::getFrame()
{
    std::unique_lock<std::mutex> lock(curFrameMutex);

    if(frameQuque.empty()) {
        conditionVar.wait(lock);
    }

    else {
        std::shared_ptr<Frame> frame = frameQuque.front();
        curFrame.swap(frame);
        frameQuque.pop_front();
//        std::cout<<curFrame->RGBImg().cols<<" "<<curFrame->RGBImg().rows<<"\n";
    }
    return curFrame;
}

void ImgIO::getImgFrmCam()
{
    while (true) {
        if(frameQuque.size() > 7)
            frameQuque.pop_front();

        cv::Mat mat;
        cap >> mat;
        float exposureTime =1;// cap.get(CV_CAP_PROP_EXPOSURE);

        //! distortion correction
        cv::Mat img;
        cv::undistort(mat, img, cameraCV(), camDistortionCV());
        std::shared_ptr<Frame> frame = std::make_shared<Frame>(img, exposureTime);
        //std::cout << "frame::rgbImg.cols" << frame->RGBImg().cols << std::endl;
        frameQuque.push_back(frame);
        conditionVar.notify_all();
    }
}
