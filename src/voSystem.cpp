#include <stdio.h>

#include "voSystem.h"

#include "ImgIO.h"
#include "Frame.h"

voSystem::voSystem()
{

}

bool voSystem::running()
{
    imgIO = std::make_shared<ImgIO>();
    trackThread = new boost::thread(std::bind(&voSystem::tracking, this));
    optThread = new boost::thread(std::bind(&voSystem::optimize, this));

    return true;
}

int voSystem::getImage(cv::Mat &input)
{
    if(currentFrame.get() != nullptr) {
        input = currentFrame->RGBImg();
        return 0;
    }

    return -1;
}

int voSystem::getPoints(std::shared_ptr<std::vector<std::shared_ptr<Eigen::Vector3f>>> &points)
{
    if(points.get() == nullptr) {
        return -1;
    }

    points->clear();
    for (int var = 0; var < 20; ++var) {
        points->push_back(std::make_shared<Eigen::Vector3f>(10.0+1.2*var, 12.0 - 3*var, 1.0));
    }

    return -1;
}

void voSystem::tracking()
{
    printf("tracking!\n");
}

void voSystem::optimize()
{
    printf("optimize!\n");
}
