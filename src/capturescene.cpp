#include <QDebug>
#include "capturescene.h"

CaptureScene::CaptureScene(QObject *parent)
    : QGraphicsScene(parent)
{
    qDebug() << "Initializing capture scene...";
    QPixmap qpix;
    mainPixmap = this->addPixmap(qpix);
}

CaptureScene::~CaptureScene()
{
    qDebug() << "Destroying capture scene...";
}

void CaptureScene::paintBackground(cv::Mat *img)
{
    mainPixmap->setPixmap(mat2pix(img));
}

QGraphicsPixmapItem *CaptureScene::getMainPixmap()
{
    return mainPixmap;
}

QPixmap CaptureScene::mat2pix(cv::Mat *img)
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
        qWarning() << "Mat2pix - unknown format!";
    }
    return QPixmap();
}
