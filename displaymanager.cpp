#include <QDebug>
#include "displaymanager.h"

void DisplayManager::addCaptureManager(CaptureManager *cap)
{
    capManager = cap;
}

void DisplayManager::displayMainPicture()
{

}

void DisplayManager::displaySmallPicture()
{

}

DisplayManager::DisplayManager()
{
    qDebug() << "Initializing display manager...";
}

DisplayManager::~DisplayManager()
{
    qDebug() << "Destroying display manager...";
}
