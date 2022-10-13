#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "maincapturescene.h"

MainCaptureScene::MainCaptureScene(QObject *parent, CaptureManager *capManager)
    : CaptureScene(parent)
{
    qDebug() << "Initializing capture scene...";
    QPixmap qpix;
    mainPixmap = this->addPixmap(qpix);
    this->capManager = capManager;

    edgePen.setColor(Qt::black);
    edgePen.setBrush(Qt::SolidPattern);
    edgeBrush.setColor(Qt::red);
    edgeBrush.setStyle(Qt::SolidPattern);
}

MainCaptureScene::~MainCaptureScene()
{
    qDebug() << "Destroying capture scene...";
}

void MainCaptureScene::drawEdgePoints()
{
    for (QPoint &point : points)
    {
        // Add edges to UI
        edges.append( this->addEllipse(point.x() - (edgePointSize / 2),
                                       point.y() - (edgePointSize / 2),
                                       edgePointSize,
                                       edgePointSize,
                                       edgePen,
                                       edgeBrush) );
    }
}

void MainCaptureScene::clearEdgePoints()
{
    for (QGraphicsEllipseItem *&item : edges)
    {
        this->removeItem(item);
    }
    edges.clear();
}

void MainCaptureScene::paintForeground()
{

}

void MainCaptureScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void MainCaptureScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void MainCaptureScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "Mouse released!";
    if(capManager->flags.testFlag(CaptureManager::capturingEdges))
    {
        if(points.count() >= 4)
        {
            points.clear();
            clearEdgePoints();
        }
        // Create a ellipse item and add it to list
        QPoint point;
        point = mouseEvent->scenePos().toPoint();
        points.append(point);
        drawEdgePoints();

        if(points.count() == 4)
        {
            clearEdgePoints();
            emit updateEdges(points);
        }
    }
}
