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
    //    startTimer(40);
}

void CameraView::setImage(const cv::Mat &newFrame)
{
    currentMap = newFrame.clone();
    update();
}

void CameraView::paintEvent(QPaintEvent *)
{
    if(currentFrame_->get() == nullptr) {
        QImage Img(640, 480, QImage::Format_RGB888);
        Img.fill(Qt::white);
        QPainter p(this);
        p.drawImage(0, 0, Img);
        p.end();
        return;
    }

    const cv::Mat Img_ = currentMap;

    if(Img_.empty()) {
        QImage Img(640, 480, QImage::Format_RGB888);
        Img.fill(Qt::blue);
        QPainter p(this);
        p.drawImage(0, 0, Img);
        p.end();
        return;
    }

    QImage Img(Img_.data, Img_.cols, Img_.rows,Img_.step, QImage::Format_RGB888);

    QPainter p(this);
    p.drawImage(0, 0, Img.rgbSwapped());
    p.end();
}

void CameraView::timerEvent(QTimerEvent *)
{
    update();
}
