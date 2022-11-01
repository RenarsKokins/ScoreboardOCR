#ifndef CAPTUREMANAGER_H
#define CAPTUREMANAGER_H
#include <QList>
#include <QFlags>
#include <QPoint>
#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

struct Camera
{
    QString name;
    QString description;
    unsigned short index;
};

class CaptureManager
{
public:
    enum Flag
    {
        captureSelected = 1,
        captureStarted = 2,
        capturingEdges = 4,
        edgesMarked = 8
    };
    Q_DECLARE_FLAGS(Flags, Flag);
    Flags flags;                // Contains capture and device related flags

    CaptureManager();
    ~CaptureManager();
    int initCapture();          // Starts video capture
    int stopCapture();          // Stops video capture
    QList<Camera> getDevices(); // Returns a QList with all current devices
    cv::Mat *getFrame();        // Returns captured frame
    bool captureFrame();        // Capture frame from camera
    void clearEdges();          // Clears edges in capture manager
    void startMarkingEdges();   // Sets up capture manager to capture edges
    void changeCurrentDeviceIndex(short); // Change current device index
    QList<QPoint> *getEdges();            // Set edges for transform
    void setEdges(QList<QPoint>);         // Set edges for transform
private:
    void updateDeviceList();    // Updates the device list

    cv::Mat frame;              // Contains the last image from capture device
    cv::VideoCapture capture;   // Current capture device

    short curDeviceIndex;           // Stores index of current selected capture device
    QList<Camera> captureDevices;   // Stores all available capture devices
    QList<QPoint> edges;            // Stores edges for transform
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CaptureManager::Flags);

#endif // CAPTUREMANAGER_H
