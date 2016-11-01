#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <memory>
#include <deque>

class Frame;

class Optimizater
{
public:
    enum OPT_TYPE {
        OPT_DEFAULT,
        OPT_G2O,
        OPT_CERES
    };

    Optimizater(OPT_TYPE optType = OPT_DEFAULT);
    void forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame);
    void forKeyFrame(std::deque<std::shared_ptr<Frame>> keyFrames);


private:

};

#endif // OPTIMIZATION_H
