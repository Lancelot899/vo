#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <QWidget>

#include <opencv2/opencv.hpp>


class Frame;

class CameraView : public QWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget *parent = 0);
    void setFrame(std::shared_ptr<Frame>& currentFrame);
    void setImage(const cv::Mat &newFrame);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

signals:

public slots:

private:
    boost::shared_mutex       currentFrameMutex;
    std::shared_ptr<Frame>   *currentFrame_;
    cv::Mat                   currentMap;
};

#endif // CAMERAVIEW_H
