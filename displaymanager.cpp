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
    if(!capManager->flags.testFlag(CaptureManager::captureStarted))
    {
        qDebug() << "Tried to display small image, but capture has not been started...";
        return;
    }
    smallImage.setPixmap(mat2pix(capManager->getFrame()));
    if(smallScene->items().count() < 1)
    {
        smallScene->addItem(&smallImage);
        smallView->fitInView(&smallImage, Qt::KeepAspectRatio);
    }
}

DisplayManager::DisplayManager(QGraphicsView *sView, QGraphicsView *mView)
{
    qDebug() << "Initializing display manager...";
    // Create scenes on which to paint/draw on
    mainScene = new QGraphicsScene();
    smallScene = new QGraphicsScene();

    // Add scenes to corresponding views
    mainView = mView;
    smallView = sView;
    mainView->setScene(mainScene);
    smallView->setScene(smallScene);
}

DisplayManager::~DisplayManager()
{
    qDebug() << "Destroying display manager...";
}

QPixmap DisplayManager::mat2pix(cv::Mat *img)
{
    switch(img->type())
    {
    case CV_8UC4:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_ARGB32);
            return QPixmap::fromImage(qimg);
            break;
        }
    case CV_8UC3:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_RGB888);
            return QPixmap::fromImage(qimg.rgbSwapped());
            break;
        }
    case CV_8UC1:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_Grayscale8);
            return QPixmap::fromImage(qimg);
            break;
        }
    default:
        qWarning() << "Mat2pix unknown format!";
    }
    return QPixmap();
}
