#include <string.h>

#include <unordered_map>
#include <fstream>

#include "setting.h"


class Setting {
public:
    friend float getVal(char *index);
    static void setVal(char* index, float val)
    {
        assert(index != nullptr);

        configueParam.insert(std::pair<char*, float>(index, val));
    }

private:
   static std::unordered_map<char*, float> configueParam;
};

std::unordered_map<char*, float> Setting::configueParam;

class Camera {
public:
    static void setCamera(char *param)
    {
        assert(param != nullptr);

        float fx = 0.0f, fy = 0.0f, cx = 0.0f, cy = 0.0f;
        sscanf(param, "[%f %f %f %f]", &fx, &fy, &cx, &cy);
        cameraCV_.create(3, 3, CV_32FC1);
        memset(cameraCV_.data, 0, 3 * 3 *sizeof(float));
        cameraCV_.at<float>(0, 0) = camera_(0, 0) = fx;
        cameraCV_.at<float>(0, 2) = camera_(0, 2) = cx;
        cameraCV_.at<float>(1, 1) = camera_(1, 1) = fy;
        cameraCV_.at<float>(1, 2) = camera_(1, 2) = cy;
        cameraCV_.at<float>(1, 1) = camera_(1, 1) = 1.0f;
    }

    friend Eigen::Matrix3f& camera();
    friend cv::Mat& cameraCV();

private:
    static Eigen::Matrix3f camera_;
    static cv::Mat cameraCV_;
};

Eigen::Matrix3f Camera::camera_;
cv::Mat Camera::cameraCV_;

class Distortion {
public:
    static void setDistortion(char *param)
    {
        assert(param != nullptr);

        float dist[5];
        sscanf(param, "[%f %f %f %f %f]", &dist[0], &dist[1], &dist[2], &dist[3], &dist[4]);
        distortionCV_.create(5, 1, CV_32FC1);
        memset(distortionCV_.data, 0, 5 * 1 *sizeof(float));

        for(int i = 0; i < 5; ++i)
            distortion_(i, 0) = distortionCV_.at<float>(i, 0) = dist[i];
    }

    friend Eigen::Matrix<float, 5, 1> &camDistortion();
    friend cv::Mat &camDistortionCV();

private:
    static Eigen::Matrix<float, 5, 1> distortion_;
    static cv::Mat distortionCV_;
};

Eigen::Matrix<float, 5, 1> Distortion::distortion_;
cv::Mat Distortion::distortionCV_;

Eigen::Matrix3f& camera() {
    return Camera::camera_;
}

cv::Mat& cameraCV() {
    return Camera::cameraCV_;
}


Eigen::Matrix<float, 5, 1> &camDistortion()
{
    return Distortion::distortion_;
}


cv::Mat &camDistortionCV()
{
    return Distortion::distortionCV_;
}



float getVal(char *index)
{
    auto it = Setting::configueParam.find(index);
    assert(it != Setting::configueParam.end());

    return it->second;
}


void getConfigueParam(const char *cfgPath) {
    char *buffer = new char[128];
    memset(buffer, 0, 128);

    std::fstream f(cfgPath);
    assert(f.is_open() == true);

    f.getline(buffer, 128);

    while(strlen(buffer)) {
        if(strstr(buffer, "# camera param")) {
            f.getline(buffer, 128);
            Camera::setCamera(buffer);
        }

        if(strstr(buffer, "# Distortion")) {
            f.getline(buffer, 128);
            Distortion::setDistortion(buffer);
        }

        f.getline(buffer, 128);
    }

}



