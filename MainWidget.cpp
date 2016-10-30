#include "MainWidget.h"
#include "src/voSystem.h"
#include "CameraView.h"
#include "MapView.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    system     = std::make_shared<voSystem>();
    mapView    = std::make_shared<MapView>();
    cameraView = std::make_shared<CameraView>();
}
