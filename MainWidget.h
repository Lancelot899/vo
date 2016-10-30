#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <memory>
#include <QWidget>

class voSystem;
class MapView;
class CameraView;


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:

private:
    std::shared_ptr<voSystem>   system;
    std::shared_ptr<CameraView> cameraView;
    std::shared_ptr<MapView>    mapView;
};

#endif // MAINWIDGET_H
