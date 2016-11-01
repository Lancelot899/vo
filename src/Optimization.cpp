#include <sophus/se3.hpp>
#include <sophus/so2.hpp>

#include "Optimization.h"


Optimizater::Optimizater(OPT_TYPE optType)
{
}

void Optimizater::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame)
{

}

void Optimizater::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames)
{

}
