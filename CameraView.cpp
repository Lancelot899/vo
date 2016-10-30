#include <QPainter>
#include <QImage>

#include "CameraView.h"

CameraView::CameraView(QWidget *parent) :
    QWidget(parent)
{
}

void CameraView::setImg(cv::Mat &img)
{
    Img_ = img;
    update();
}

void CameraView::paintEvent(QPaintEvent *)
{

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
