#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "maincapturescene.h"

MainCaptureScene::MainCaptureScene(QObject *parent, CaptureManager *capManager, RecognitionManager *recManager)
    : CaptureScene(parent)
{
    qDebug() << "Initializing capture scene...";
    QPixmap qpix;
    mainPixmap = this->addPixmap(qpix);
    this->capManager = capManager;
    this->recManager = recManager;

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
    if(recManager->flags.testFlag(RecognitionManager::selectingSelection))
    {
        // Set X, Y, width and height correctly from given coords

        int x = 0, y = 0, w = 0, h = 0;
        if(mouseStartPos.x() < mouseEvent->scenePos().x())
        {
            x = mouseStartPos.x();
            w = mouseEvent->scenePos().x() - x;
        } else
        {
            x = mouseEvent->scenePos().x();
            w = mouseStartPos.x() - x;
        }

        if(mouseStartPos.x() < mouseEvent->scenePos().y())
        {
            y = mouseStartPos.y();
            h = mouseEvent->scenePos().y() - y;
        } else
        {
            y = mouseEvent->scenePos().y();
            h = mouseStartPos.y() - y;
        }
        selectionBoxes.last()->setRect(x, y, w, h);
    }
}

void MainCaptureScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "Mouse pressed!";
    mouseStartPos = mouseEvent->scenePos().toPoint();
    if(recManager->flags.testFlag(RecognitionManager::selectingSelection))
    {
        // Set a random brush color
        QPen pen;
        QBrush brush;
        pen.setColor(Qt::black);
        pen.setBrush(Qt::SolidPattern);
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);

        // Add a graphical square to scene
        selectionBoxes.append( this->addRect(mouseStartPos.x(),
                                             mouseStartPos.y(),
                                             0,
                                             0,
                                             pen,
                                             brush) );
        selectionBoxes.last()->setOpacity(0.5);
    }
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
    if(recManager->flags.testFlag(RecognitionManager::selectingSelection))
    {
        emit updateSelectionCoordinates(selectionBoxes.last()->rect().toRect());
        recManager->flags.setFlag(RecognitionManager::selectingSelection, false);
    }
}

void MainCaptureScene::removeSelection(int i)
{
    this->removeItem(selectionBoxes.at(i));
    selectionBoxes.removeAt(i);
}
