#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include "filtermanager.h"
#include "capturemanager.h"

class MainWorker : public QObject
{
    Q_OBJECT
public:
    MainWorker();
    ~MainWorker();
    void addFilterManager(FilterManager *);
    void addCaptureManager(CaptureManager *);

public slots:
    void doWork();                  // Run an endless loop which does image capture, detection, filters, etc.

signals:
    void setMainImage(cv::Mat *);   // Signal to set main image
    void setSmallImage(cv::Mat *);   // Signal to set main image

private:
    FilterManager *filManager;     // Pointer to FilterManager
    CaptureManager *capManager;     // Pointer to CaptureManager
};

#endif // MAINWORKER_H
