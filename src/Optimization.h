#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <memory>
#include <deque>

class Frame;

class OptimizaterImpl;

class Optimizater
{
public:
    enum OPT_TYPE {
        OPT_DEFAULT,
        OPT_G2O,
        OPT_CERES
    };

    Optimizater(OPT_TYPE optType = OPT_DEFAULT);
    bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai = 0.0f, float aj = 0.0f, float bi = 0.0f, float bj = 0.0f);
    bool forKeyFrame(std::deque<std::shared_ptr<Frame>> keyFrames);


private:
    std::shared_ptr<OptimizaterImpl> impl;
};

#endif // OPTIMIZATION_H
