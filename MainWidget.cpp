#include <QGridLayout>

#include "MainWidget.h"
#include "src/voSystem.h"
#include "CameraView.h"
#include "MapView.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    btnStart      = new QPushButton;
    btnCamView    = new QPushButton;
    system        = std::make_shared<voSystem>();
    mapView       = std::make_shared<MapView>();
    cameraView    = std::make_shared<CameraView>();

    btnStart->setText("start");
    connect(btnStart, SIGNAL(clicked()), this, SLOT(actStart()));
    connect(btnCamView, SIGNAL(clicked()), this, SLOT(actCamShow()));

    btnCamView->setText("camera show");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(mapView.get(), 0, 0, 3, 3);
    mainLayout->addWidget(btnCamView, 3, 1, 1, 1);
    mainLayout->addWidget(btnStart, 3, 2, 1, 1);
    cameraView->setFixedSize(640, 480);
    cameraView->setWindowTitle("camera view");


    grapFrame = new boost::thread(std::bind(&MainWidget::refresh_, this));
    this->setLayout(mainLayout);
}

void MainWidget::paintEvent(QPaintEvent *)
{
    cv::Mat currentImage;
    system->getImage(currentImage);
    if(!currentImage.empty())
        cameraView->setImg(currentImage);

    system->getPoints(mapView->getPoints());
}

void MainWidget::actStart()
{
    cameraView->show();
    system->running();
}

void MainWidget::actCamShow()
{
    if(cameraView->isHidden())
        cameraView->show();
    else
        cameraView->hide();
}
