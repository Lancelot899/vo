#include "MapView.h"

MapView::MapView(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags flags) :
    QGLViewer(parent, shareWidget, flags)
{
    points = std::make_shared<std::vector<std::shared_ptr<Eigen::Vector3f>>>();
}

void MapView::draw()
{
    if(!points->size())
        return;
    glBegin(GL_POINTS);
    for (auto it = points->begin(); it != points->end(); ++it) {
        glVertex3f((*(*it))(0), (*(*it))(1), (*(*it))(2));
    }
    glEnd();
}
