#include "MainView.h"

MainView::MainView(QWidget* parent)
    : QGraphicsView(parent)
{
    //resize(800, 500);
}

void MainView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
