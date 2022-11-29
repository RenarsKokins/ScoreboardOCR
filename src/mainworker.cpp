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
        // Limit capture updates to a certain FPS
        long timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if(timeElapsed < (1000/fps))
        {
            //QThread::msleep((1000/fps) - timeElapsed);
            capManager->captureFrame(); // We need to constantly capture from camera, so buffer is always fresh
            continue;
        }
        lastTime = std::chrono::steady_clock::now();

        // Check if we can get image, update small scene
        if(!capManager->captureFrame())
            continue;
        emit setSmallImage(capManager->getFrame());

        // Show main image without filters if edges haven't been marked
        if(!capManager->flags.testFlag(CaptureManager::edgesMarked)) {
            emit setMainImage(capManager->getFrame());
            continue;
        }
        // Apply filters and set main image to it
        filManager->createImageWithFilters(capManager->getFrame(), *capManager->getEdges());
        emit setMainImage(filManager->getImageWithFilters());

        // Do number detection and recognition
        if(!recManager->selectionsAdded())
            continue;
        recManager->findNumbers(filManager->getImageWithFilters());
        if(recManager->flags.testFlag(RecognitionManager::svmLoaded) && !recManager->flags.testFlag(RecognitionManager::saveNumbers))
            recManager->recognizeNumbers();

        // Save recognized numbers to a file
        outManager->saveRecognizedNumbers();

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

void MainWorker::addOutputManager(OutputManager *out)
{
    outManager = out;
}
