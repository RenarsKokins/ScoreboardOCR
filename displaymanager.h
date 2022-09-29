#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QImage>
#include "capturemanager.h"

class DisplayManager
{
public:
    DisplayManager();
    ~DisplayManager();
    void addCaptureManager(CaptureManager *);   // Add pointer to CaptureManager object
    void displayMainPicture();                  // Get relevant image and set it as main image
    void displaySmallPicture();                 // Get relevant image and set it as small (thumbnail) image
private:
    CaptureManager *capManager;     // Pointer to main capture manager
    QImage mainImg;
    QImage smallImg;
};

#endif // DISPLAYMANAGER_H
