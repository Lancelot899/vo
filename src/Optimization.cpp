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

        return *this;
    }

    Jac& operator *=(float val) {
        rij_ai *= val;
        rij_aj *= val;
        rij_bi *= val;
        rij_bj *= val;
        rij_di *= val;

        return *this;
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

        return *this;
    }

    JacKf& operator *=(float val) {
        jac *= val;
        rij_xi *= val;
        rij_xj *= val;

        return *this;
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

        return *this;
    }

    JacKff& operator *=(float val) {
        jac *= val;
        rij_xij *= val;

        return *this;
    }
};



inline Jac calcJac(std::shared_ptr<Frame>& frame_i, std::shared_ptr<Frame>& frame_j, float ai, float aj, float bi, float bj) {
    Jac jac;

}


class OptimizaterImpl {
public:
    virtual bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai = 0.0f, float aj = 0.0f, float bi = 0.0f, float bj = 0.0f);
    virtual bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};

bool OptimizaterImpl::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai, float aj, float bi, float bj) {
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
    bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai = 0.0f, float aj = 0.0f, float bi = 0.0f, float bj = 0.0f);
    bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};

bool OptimizaterImplG2O::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames) {
    return true;
}


bool OptimizaterImplG2O::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai, float aj, float bi, float bj) {
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class OptimizaterImplCeres : public OptimizaterImpl {
public:
    bool forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai = 0.0f, float aj = 0.0f, float bi = 0.0f, float bj = 0.0f);
    bool forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames);
};


bool OptimizaterImplCeres::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames) {
    return true;
}

bool OptimizaterImplCeres::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai, float aj, float bi, float bj) {
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

bool Optimizater::forTracking(std::shared_ptr<Frame> kFrame, std::shared_ptr<Frame> frame, float ai, float aj, float bi, float bj)
{
    return impl->forTracking(kFrame, frame, ai, aj, bi, bj);
}

bool Optimizater::forKeyFrame(std::deque<std::shared_ptr<Frame> > keyFrames)
{
    return impl->forKeyFrame(keyFrames);
}

