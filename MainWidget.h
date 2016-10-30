#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <memory>

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

signals:

public slots:
    void actStart();
    void actCamShow();

private:
    std::shared_ptr<voSystem>   system;
    std::shared_ptr<CameraView> cameraView;
    std::shared_ptr<MapView>    mapView;
    QPushButton                 *btnStart;
    QPushButton                 *btnCamView;
};

#endif // MAINWIDGET_H
