#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <memory>

#include <Eigen/Dense>

#include <QGLViewer/qglviewer.h>

class MapView : public QGLViewer
{
    Q_OBJECT
public:
    typedef std::shared_ptr<std::vector<std::shared_ptr<Eigen::Vector3f>>> pointsType;
    explicit MapView(QWidget* parent=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0);
    pointsType& getPoints() { return points;}


    void draw();

signals:

public slots:

private:
    pointsType points;
};

#endif // MAPVIEW_H
