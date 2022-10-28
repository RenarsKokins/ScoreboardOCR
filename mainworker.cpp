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
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        // check if interrupt has happened, if not, loop
        // get image from camera
        // apply filters to that image
        // do detection
        // save data
        // signal small and main image display

        // QThread::msleep(500);

        capManager->captureFrame();
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
