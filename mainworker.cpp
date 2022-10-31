#include <chrono>
#include <QDebug>
#include <QThread>
#include "mainworker.h"

MainWorker::MainWorker()
{
    qDebug() << "Initializing main worker...";
}

MainWorker::~MainWorker()
{
    qDebug() << "Destroying main worker...";
}

void MainWorker::doWork()
{
    std::chrono::time_point lastTime = std::chrono::steady_clock::now();
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        // check if interrupt has happened, if not, loop
        // get image from camera
        // apply filters to that image
        // do detection
        // save data
        // signal small and main image display

        // Limit capture updates to a certain FPS
        long timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if(timeElapsed < (1000/fps))
        {
            QThread::msleep((1000/fps) - timeElapsed);
            continue;
        }
        lastTime = std::chrono::steady_clock::now();

        if(!capManager->captureFrame())
            continue;
        emit setSmallImage(capManager->getFrame());

        if(!capManager->flags.testFlag(CaptureManager::edgesMarked)) {
            emit setMainImage(capManager->getFrame());
            continue;
        }
        filManager->createImageWithFilters(capManager->getFrame(), *capManager->getEdges());
        emit setMainImage(filManager->getImageWithFilters());

        if(!recManager->selectionsAdded())
            continue;
        recManager->findNumbers(filManager->getImageWithFilters());
        lastTime = std::chrono::steady_clock::now();
    }
}

void MainWorker::addCaptureManager(CaptureManager *cap)
{
    capManager = cap;
}

void MainWorker::addFilterManager(FilterManager *fil)
{
    filManager = fil;
}

void MainWorker::addRecognitionManager(RecognitionManager *rec)
{
    recManager = rec;
}
