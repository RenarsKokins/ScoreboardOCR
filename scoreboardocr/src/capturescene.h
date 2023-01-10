#ifndef CAPTURESCENE_H
#define CAPTURESCENE_H

#include <QGraphicsScene>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <QGraphicsPixmapItem>

class CaptureScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CaptureScene(QObject *parent = nullptr);
    ~CaptureScene();
    void paintBackground(cv::Mat *img);     // Draw background image
    QGraphicsPixmapItem *getMainPixmap();   // Returns pointer to main pixmap

protected:
    QGraphicsPixmapItem *mainPixmap;    // Main pixmap with capture image

private:
    QPixmap mat2pix(cv::Mat *img);      // Convert mat image to pixmap
};

#endif // CAPTURESCENE_H
