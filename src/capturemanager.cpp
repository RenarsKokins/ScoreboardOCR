#include <QDebug>
#include <QCameraDevice>
#include <QMediaDevices>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "capturemanager.h"

void CaptureManager::changeCurrentDeviceIndex(short index)
{
    curDeviceIndex = index;
    if (index < 0)
        flags.setFlag(CaptureManager::captureSelected, false);
    else
        flags.setFlag(CaptureManager::captureSelected, true);
}

bool CaptureManager::captureFrame()
{
    capture.read(frame);
    if (frame.empty())
    {
        qWarning() << "BLANK frame grabbed from capture device!";
        return 0;
    }
    return 1;
}

cv::Mat *CaptureManager::getFrame()
{
    return &frame;
}

int CaptureManager::initCapture()
{
    if (curDeviceIndex < 0)
    {
        qWarning() << "No capture device selected!";
        flags.setFlag(CaptureManager::captureSelected, false);
        return -1;
    }
    capture.open(captureDevices[curDeviceIndex].index, cv::CAP_ANY); // Start the capture
    if (!capture.isOpened())
    {
        qWarning() << "Cannot open capture device!";
        return 0;
    }
    flags.setFlag(CaptureManager::captureStarted, true);
    qDebug() << "Capture initiated!";
    return 1;
}

int CaptureManager::stopCapture()
{
    flags.setFlag(CaptureManager::captureStarted, false);
    if (!capture.isOpened())
    {
        qWarning() << "No device has been opened, so no need to close it!";
        return 0;
    }
    capture.release();
    qDebug() << "Capture stopped!";
    return 1;
}

QList<Camera> CaptureManager::getDevices()
{
    updateDeviceList();
    return captureDevices;
}

void CaptureManager::updateDeviceList()
{
    captureDevices.clear();
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.count() < 1)
    {
        qDebug() << "No capture devices found!";
        flags.setFlag(CaptureManager::captureSelected, false);
        return;
    }

    // Add detected cameras to captureDevices list.
    // Camera indexes can vary on different OSes.
    Camera cam;
    for (unsigned short i = 0; i < cameras.count(); i++)
    {
        qDebug() << "Found device:" << cameras[i].id() << "|" << cameras[i].description();
        cam.name = cameras[i].id();                   // Device name
        cam.description = cameras[i].description();   // Device description
#ifdef Q_OS_LINUX
        // Index for camera access on linux gets extracted from cam name

        if (cam.name.length() > 10)
        {
            cam.index = cam.name.mid(10).toInt();
        } else
        {
            cam.index = i;
            qDebug() << "Linux broke :(";
        }
#else
        cam.index = i;
#endif
        captureDevices.append(cam);
    }
}

void CaptureManager::startMarkingEdges()
{
    flags.setFlag(CaptureManager::capturingEdges, true);
    flags.setFlag(CaptureManager::edgesMarked, false);
}

void CaptureManager::clearEdges()
{
    flags.setFlag(CaptureManager::capturingEdges, false);
    flags.setFlag(CaptureManager::edgesMarked, false);
}

void CaptureManager::setEdges(QList<QPoint> points)
{
    edges.clear();
    for (QPoint &point : points)
    {
        edges.append(point);
    }

    // Sort edges list by Y value (ascending)
    for (int j = 0; j < 3; j++)
    {
        if(edges[j].y() > edges[j+1].y())
        {
            edges.swapItemsAt(j, j+1);
            j = -1;
        }
    }

    // Sort edges list by X value
    if(edges[0].x() > edges[1].x())
    {
        edges.swapItemsAt(0, 1);
    }
    if(edges[2].x() > edges[3].x())
    {
        edges.swapItemsAt(2, 3);
    }

    flags.setFlag(CaptureManager::capturingEdges, false);
    flags.setFlag(CaptureManager::edgesMarked, true);
}

QList<QPoint> *CaptureManager::getEdges()
{
    return &edges;
}

CaptureManager::CaptureManager()
{
    qDebug() << "Initiating caputure manager...";
    curDeviceIndex = -1;
}

CaptureManager::~CaptureManager()
{
    stopCapture();
    qDebug() << "Destroying caputure manager...";
}
