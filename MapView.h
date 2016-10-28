#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGLViewer/qglviewer.h>

class MapView : public QGLViewer
{
    Q_OBJECT
public:
    explicit MapView(QWidget* parent=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0);

signals:

public slots:

};

#endif // MAPVIEW_H
