#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include <opencv2/opencv.hpp>


class CameraView : public QWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget *parent = 0);
    void setImg(cv::Mat &img);
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    cv::Mat Img_;
};

#endif // CAMERAVIEW_H
