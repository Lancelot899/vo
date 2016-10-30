#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <memory>

#include <QWidget>
#include <opencv2/opencv.hpp>


class Frame;

class CameraView : public QWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget *parent = 0);
    void setFrame(std::shared_ptr<Frame>& currentFrame);
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    std::shared_ptr<Frame> *currentFrame_;
};

#endif // CAMERAVIEW_H
