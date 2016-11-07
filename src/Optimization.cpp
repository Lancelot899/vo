#include <thread>

#include <Eigen/Dense>

#include <sophus/se3.hpp>
#include <sophus/so2.hpp>


#include "Optimization.h"
#include "Frame.h"

typedef Eigen::Matrix<float, 1, 6> Vector6fRow;
typedef Eigen::Matrix<float, 6, 6> HType;
typedef Eigen::Matrix<float, 6, 1> ErrType;


struct Hessian_ {
    HType Hii;
    HType Hij;
    HType Hji;
    HType Hjj;
    ErrType ei;
    ErrType ej;
    Hessian_() {
        Hii.setOnes();
        Hij.setOnes();
        Hji.setOnes();
        Hjj.setOnes();
        ei.setOnes();
        ej.setOnes();
    }

    Hessian_& operator +=(Hessian_& oth) {
        Hii += oth.Hii;
        Hij += oth.Hij;
        Hji += oth.Hji;
        Hjj += oth.Hjj;
        ei  += oth.ei;
        ej  += oth.ej;

        return *this;
    }

    Hessian_& operator =(Hessian_& oth) {
        Hii = oth.Hii;
        Hij = oth.Hij;
        Hji = oth.Hji;
        Hjj = oth.Hjj;
        ei  = oth.ei;
        ej  = oth.ej;

        return *this;
    }
};

struct HessianBlock {
    int  i;
    int  j;
    Hessian_ hessian;
    HessianBlock(int i, int j) {
        this->i = i;
        this->j = j;
    }

    HessianBlock() = delete;

    HessianBlock(HessianBlock& oth) {
        i = oth.i;
        j = oth.j;
        hessian = oth.hessian;
    }

    HessianBlock& operator +=(HessianBlock& oth) {
        hessian += oth.hessian;
        return *this;
    }

};

struct Jac {
    int i, j;
    float rij_bj;
    float rij_bi;
    float rij_ai;
    float rij_aj;
    float rij_di;

    Jac() {
        i = j = -1;
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
    Vector6fRow rij_xji;

    JacKff() {
        rij_xji.setOnes();
    }

    JacKff& operator *(float val) {
        jac *= val;
        rij_xji *= val;

        return *this;
    }

    JacKff& operator *=(float val) {
        jac *= val;
        rij_xji *= val;

        return *this;
    }
};

inline Jac calcJac_p(int i, int j, float ti, float tj, float Ii, float di, float dj,
                     int ui, int vi, float Iju,
                     float Ijv, float ai, float aj, float bi, Sophus::SE3f &xji,
                     float fx, float fy, float cx, float cy) {
    Jac jac;
    jac.i = i; jac.j = j;
    jac.rij_bi = tj * std::exp(aj) / ti / std::exp(ai);
    jac.rij_ai = (Ii - bi) * tj * std::exp(aj) / ti / std::exp(ai);
    jac.rij_aj = -(Ii - bi) * tj * std::exp(aj) / ti / std::exp(ai);
    Eigen::Vector3f dleft(fx * Iju, fy * Ijv, cx * Iju + cy * Ijv);
    Eigen::Vector3f dright((ui - cx) / fx, (vi - cy) / fy, 1);
    dright.noalias() = xji.so3() * dright + xji.translation() / di;
    jac.rij_di = dleft.dot(dright) / dj;
    return jac;
}

JacKff calcJacForTracking(std::shared_ptr<Frame> frame_i, std::shared_ptr<Frame> frame_j, int u, int v, float ai, float aj, float bi, float _di_ = -1.0f) {
    JacKff Jaccobi;
    float ti = frame_i->exposureTime();
    float tj = frame_j->exposureTime();
    float Ii = frame_i->Image()[u * frame_i->height() + v];

    float di = 0.0f;
    if(_di_ < -0.5f) di = _di_;
    else di = frame_i->getDepth(u, v);

    Eigen::Vector3f pi = di * camInv() * Eigen::Vector3f(u, v, 1) ;
    Sophus::SE3f xji = frame_j->pose() * frame_i->pose().inverse();
    Eigen::Vector3f pj = xji.so3() * pi + xji.translation();
    int uj = pj[0] / pj[2];
    int vj = pj[1] / pj[2];
    Eigen::Vector3f gradIj = frame_j->gradients()[uj * frame_j->height() + vj];

    Jaccobi.jac = calcJac_p(frame_i->getID(), frame_j->getID(), ti, tj, Ii, di, pj[2], u, v,
            gradIj[0], gradIj[1], ai, aj, bi, xji,
            frame_i->fx(), frame_i->fy(), frame_i->cx(), frame_i->cy());

    Jaccobi.rij_xji(0, 0) = frame_i->fx() / di * gradIj[0];
    Jaccobi.rij_xji(0, 1) = frame_i->fy() / di * gradIj[1];
    Jaccobi.rij_xji(0, 2) = -(pi[1] * frame_i->fy() * gradIj[1] + pi[0] * frame_i->fy() * gradIj[0]) / di / di;
    Jaccobi.rij_xji(0, 3) = frame_i->fy() * gradIj[1] + pi[1] / pi[2] / pi[2] * (pi[0] * frame_i->fx() * gradIj[0] + pi[1] * frame_i->fy() * gradIj[1]);
    Jaccobi.rij_xji(0, 4) = -frame_i->fx() * gradIj[0] - pi[0] / pi[2] / pi[2] * (pi[0] * frame_i->fx() * gradIj[0] + pi[1] * frame_i->fy() * gradIj[1]);
    Jaccobi.rij_xji(0, 5) = (pi[1] * frame_i->fy() * gradIj[0] - pi[0] * frame_i->fx() * gradIj[1]) / pi[2];

    return Jaccobi;
}




JacKf calcJacForOpt(std::shared_ptr<Frame> frame_i, std::shared_ptr<Frame> frame_j, int u, int v, float ai, float aj, float bi, float _di_ = -1.0f) {
    JacKf Jaccobi;
    float ti = frame_i->exposureTime();
    float tj = frame_j->exposureTime();
    float Ii = frame_i->Image()[u * frame_i->height() + v];

    float di = 0.0f;
    if(_di_ < -0.5f) di = _di_;
    else di = frame_i->getDepth(u, v);

    Eigen::Vector3f pi = di * camInv() * Eigen::Vector3f(u, v, 1) ;
    Sophus::SE3f xji = frame_j->pose() * frame_i->pose().inverse();
    Eigen::Vector3f pj = xji.so3() * pi + xji.translation();
    int uj = pj[0] / pj[2];
    int vj = pj[1] / pj[2];
    Eigen::Vector3f gradIj = frame_j->gradients()[uj * frame_j->height() + vj];

    Jaccobi.jac = calcJac_p(frame_i->getID(), frame_j->getID(), ti, tj, Ii, di, pj[2], u, v,
            gradIj[0], gradIj[1], ai, aj, bi, xji,
            frame_i->fx(), frame_i->fy(), frame_i->cx(), frame_i->cy());

    Vector6fRow rij_xji;

    rij_xji(0, 0) = frame_i->fx() / di * gradIj[0];
    rij_xji(0, 1) = frame_i->fy() / di * gradIj[1];
    rij_xji(0, 2) = -(pi[1] * frame_i->fy() * gradIj[1] + pi[0] * frame_i->fy() * gradIj[0]) / di / di;
    rij_xji(0, 3) = frame_i->fy() * gradIj[1] + pi[1] / pi[2] / pi[2] * (pi[0] * frame_i->fx() * gradIj[0] + pi[1] * frame_i->fy() * gradIj[1]);
    rij_xji(0, 4) = -frame_i->fx() * gradIj[0] - pi[0] / pi[2] / pi[2] * (pi[0] * frame_i->fx() * gradIj[0] + pi[1] * frame_i->fy() * gradIj[1]);
    rij_xji(0, 5) = (pi[1] * frame_i->fy() * gradIj[0] - pi[0] * frame_i->fx() * gradIj[1]) / pi[2];

    Eigen::Matrix<float, 6, 6> mathJ;
    mathJ.block(0, 0, 3, 3) = Sophus::SO3f::hat(xji.so3().log());
    mathJ.block(0, 3, 3, 3) = Sophus::SO3f::hat(xji.translation());
    mathJ.block(3, 0, 3, 3) = Eigen::Matrix3f::Zero(3, 3);
    mathJ.block(3, 3, 3, 3) = Sophus::SO3f::hat(xji.so3().log());
    mathJ = 0.5 * mathJ + Eigen::Matrix<float, 6, 6>::Identity();

    Jaccobi.rij_xi.noalias() = rij_xji * mathJ * frame_i->pose().inverse().Adj();
    Jaccobi.rij_xj.noalias() = -rij_xji * mathJ * frame_i->pose().inverse().Adj();

    return Jaccobi;
}

inline Hessian_ calcBlockForTracking(JacKff& jac) {
    Hessian_ hessian;

    return hessian;
}

void BuildBlockAtPointTracking(int u, int v, std::shared_ptr<Frame> framei, std::shared_ptr<Frame> framej, HessianBlock &block) {
    auto it = framei->obsPoints().find(u * framei->height() + v);
    if(it == framei->obsPoints().end())
        return;

    while(it->second.isDepthBusy) std::this_thread::yield();
    JacKff jac = calcJacForTracking(framei, framej, u, v, framei->a(), framej->a(), framei->b(), it->second.depth);
    const std::vector< cv::Point2i > &neighber = it->second.neighber;

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

