#include <Eigen/Dense>

#include <sophus/se3.hpp>
#include <sophus/so2.hpp>


#include "Optimization.h"
#include "Frame.h"

typedef Eigen::Matrix<float, 1, 6> Vector6fRow;

struct Jac {
    float rij_bj;
    float rij_bi;
    float rij_ai;
    float rij_aj;
    float rij_di;

    Jac() {
        rij_bj = -1.0f;
        rij_bi = 0.0f;
        rij_ai = 0.0f;
        rij_aj = 0.0f;
        rij_di = 0.0f;
    }

    Jac& operator *(float val) {
        rij_ai *= val;
        rij_aj *= val;
        rij_bi *= val;
        rij_bj *= val;
        rij_di *= val;
    }

    Jac& operator *=(float val) {
        rij_ai *= val;
        rij_aj *= val;
        rij_bi *= val;
        rij_bj *= val;
        rij_di *= val;
    }

};

struct JacKf {
    Jac jac;
    Vector6fRow rij_xi;
    Vector6fRow rij_xj;

    JacKf() {
        rij_xi.setOnes();
        rij_xj.setOnes();
    }

    JacKf& operator *(float val) {
        jac *= val;
        rij_xi *= val;
        rij_xj *= val;
    }

    JacKf& operator *=(float val) {
        jac *= val;
        rij_xi *= val;
        rij_xj *= val;
    }
};

struct JacKff {
    Jac jac;
    Vector6fRow rij_xij;

    JacKff() {
        rij_xij.setOnes();
    }

    JacKff& operator *(float val) {
        jac *= val;
        rij_xij *= val;
    }

    JacKff& operator *=(float val) {
        jac *= val;
        rij_xij *= val;
    }
};



inline Jac calcJac(std::shared_ptr<Frame>& frame_i, std::shared_ptr<Frame>& frame_j) {
    Jac jac;

}


class OptimizaterImpl {
public:
    virtual bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame);
    virtual bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};

bool OptimizaterImpl::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame) {
    return true;
}


bool OptimizaterImpl::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames) {
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class OptimizaterImplG2O : public OptimizaterImpl {
public:
    bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame);
    bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};

bool OptimizaterImplG2O::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames) {
    return true;
}


bool OptimizaterImplG2O::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame) {
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class OptimizaterImplCeres : public OptimizaterImpl {
public:
    bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame);
    bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};


bool OptimizaterImplCeres::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames) {
    return true;
}

bool OptimizaterImplCeres::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame) {
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



Optimizater::Optimizater(OPT_TYPE optType)
{
    switch (optType) {
    case OPT_DEFAULT:
        impl = std::make_shared<OptimizaterImpl>();
        break;
    case OPT_G2O:
        impl = std::make_shared<OptimizaterImplG2O>();
        break;
    case OPT_CERES:
        impl = std::make_shared<OptimizaterImplCeres>();
        break;
    default:
        break;
    }
}

bool Optimizater::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame)
{
    return impl->forTracking(kFrame, frame);
}

bool Optimizater::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames)
{
    return impl->forKeyFrame(keyFrames);
}
