#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "capturemanager.h"

class DisplayManager
{
public:
    DisplayManager(QGraphicsView *, QGraphicsView *);
    ~DisplayManager();
    void addCaptureManager(CaptureManager *);   // Add pointer to CaptureManager object
    void displayMainPicture();                  // Get relevant image and set it as main image
    void displaySmallPicture();                 // Get relevant image and set it as small (thumbnail) image
private:
    CaptureManager *capManager;     // Pointer to main capture manager
    QGraphicsView  *mainView;       // Pointer to main image view widget in our UI
    QGraphicsView  *smallView;      // Pointer to small image view widget in our UI
    QGraphicsScene *mainScene;      // Pointer to main scene
    QGraphicsScene *smallScene;     // Pointer to small scene
    QGraphicsPixmapItem smallImage; // Small image
    QGraphicsPixmapItem mainImage;  // Main image

    QPixmap mat2pix(cv::Mat *);     // Convert mat image to pixmap
};

#endif // DISPLAYMANAGER_H
