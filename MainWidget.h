#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <QWidget>
#include <QPushButton>

class voSystem;
class MapView;
class CameraView;


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void refresh_(){
        while (true) {
            update();
            usleep(2000);
        }

    }

signals:

public slots:
    void actStart();
    void actCamShow();

private:
    std::shared_ptr<voSystem>   system;
    std::shared_ptr<CameraView> cameraView;
    std::shared_ptr<MapView>    mapView;

    boost::thread               *grapFrame;

    QPushButton                 *btnStart;
    QPushButton                 *btnCamView;
};

#endif // MAINWIDGET_H
