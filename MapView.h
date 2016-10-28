#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGLViewer/qglviewer.h>

class MapView : public QGLViewer
{
    Q_OBJECT
public:
    explicit MapView(QObject *parent = 0);

signals:

public slots:

};

#endif // MAPVIEW_H
