#ifndef MAINCAPTURESCENE_H
#define MAINCAPTURESCENE_H

#include <QPen>
#include <QPoint>
#include <QBrush>
#include "capturescene.h"
#include "capturemanager.h"
#include "recognitionmanager.h"

class MainCaptureScene : public CaptureScene
{
    Q_OBJECT

public:
    MainCaptureScene(QObject *parent = nullptr, CaptureManager *capManager = nullptr, RecognitionManager *recManager = nullptr);
    ~MainCaptureScene();
    void drawEdgePoints();              // Draws points from points list
    void paintForeground();             // Draw edges, detection boxes, etc.
    void clearEdgePoints();             // Clears points which are drawn
    void removeSelection(int);          // Remove selection at index
    void stopShowingSelections();       // Make selections invisible
    void startShowingSelections();      // Make selections visible

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

signals:
    void updateEdges(QList<QPoint>);        // Signal when all 4 edges have been marked
    void updateSelectionCoordinates(QRect); // Signal when coordinates have been marked for selection

private:
    QPoint mouseStartPos;               // Mouse starting position (on click)
    QList<QPoint> points;               // A list of points for image transforamtion
    CaptureManager *capManager;         // Pointer to capture manager
    int edgePointSize = 10;             // Size of ellipse to draw
    RecognitionManager *recManager;     // Pointer to recognition maanger
    QList<QGraphicsEllipseItem *> edges;// A list of edges (used for UI only)
    QList<QGraphicsRectItem *> selectionBoxes;  // A list of selection boxes (used for UI only)

    QPen edgePen;                       // Pen used for edge drawing
    QBrush edgeBrush;                   // Brush used for edge drawing

};

#endif // MAINCAPTURESCENE_H
