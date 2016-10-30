#include <QPainter>
#include <QImage>

#include <opencv2/opencv.hpp>

#include "CameraView.h"
#include "src/Frame.h"

CameraView::CameraView(QWidget *parent) :
    QWidget(parent)
{
    currentFrame_ = nullptr;
}

void CameraView::setFrame(std::shared_ptr<Frame>& currentFrame)
{
    currentFrame_ = &currentFrame;
}

void CameraView::paintEvent(QPaintEvent *)
{
    if(currentFrame_ == nullptr) {
        QImage Img(640, 480, QImage::Format_RGB888);
        Img.fill(Qt::white);
        QPainter p(this);
        p.drawImage(0, 0, Img);
        p.end();
        return;
    }

    const cv::Mat& Img_ = (*currentFrame_)->RGBImg();

    if(Img_.empty()) {
        QImage Img(640, 480, QImage::Format_RGB888);
        Img.fill(Qt::white);
        QPainter p(this);
        p.drawImage(0, 0, Img);
        p.end();
        return;
    }

    QImage Img(Img_.data, Img_.cols, Img_.rows, QImage::Format_RGB888);
    QPainter p(this);
    p.drawImage(0, 0, Img);
    p.end();
}
