#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include "filtermanager.h"
#include "capturemanager.h"
#include "recognitionmanager.h"

class MainWorker : public QObject
{
    Q_OBJECT
public:
    MainWorker();
    ~MainWorker();
    void addFilterManager(FilterManager *);
    void addCaptureManager(CaptureManager *);
    void addRecognitionManager(RecognitionManager *);

public slots:
    void doWork();                  // Run an endless loop which does image capture, detection, filters, etc.

signals:
    void setMainImage(cv::Mat *);   // Signal to set main image
    void setSmallImage(cv::Mat *);  // Signal to set main image

private:
    FilterManager *filManager;      // Pointer to FilterManager
    CaptureManager *capManager;     // Pointer to CaptureManager
    RecognitionManager *recManager; // Pointer to RecognitionManager
};

#endif // MAINWORKER_H
