#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>

class CameraView : public QWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget *parent = 0);
    void setImg();


signals:

public slots:

};

#endif // CAMERAVIEW_H
