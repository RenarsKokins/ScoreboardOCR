#ifndef MAINCAPTURESCENE_H
#define MAINCAPTURESCENE_H

#include <QPen>
#include <QPoint>
#include <QBrush>
#include "capturescene.h"
#include "capturemanager.h"

class MainCaptureScene : public CaptureScene
{
    Q_OBJECT

public:
    MainCaptureScene(QObject *parent = nullptr, CaptureManager *capManager = nullptr);
    ~MainCaptureScene();
    void paintForeground();         // Draw edges, detection boxes, etc.
    void drawEdgePoints();              // Draws points from points list
    void clearEdgePoints();             // Clears points which are drawn

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

signals:
    void updateEdges(QList<QPoint>);    // Signal when all 4 edges have been marked

private:
    QList<QPoint> points;               // A list of points for image transforamtion
    CaptureManager *capManager;         // Pointer to capture manager
    const int edgePointSize = 10;       // Size of ellipse to draw
    QList<QGraphicsEllipseItem *> edges;// A list of edges (used for UI only)

    QPen edgePen;                       // Pen used for edge drawing
    QBrush edgeBrush;                   // Brush used for edge drawing

};

#endif // MAINCAPTURESCENE_H
