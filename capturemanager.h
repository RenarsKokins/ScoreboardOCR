#ifndef CAPTUREMANAGER_H
#define CAPTUREMANAGER_H
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <QString>
#include <QList>
#include <QFlags>

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
        edgesMarked = 4
    };
    Q_DECLARE_FLAGS(Flags, Flag);

    Flags flags;                // Contains capture and device related flags

    CaptureManager();
    ~CaptureManager();
    int initCapture();          // Starts video capture
    int stopCapture();          // Stops video capture
    QList<Camera> getDevices(); // Returns a QList with all current devices
    cv::Mat *getFrame();        // Returns captured frame
    void captureFrame();        // Capture frame from camera
    void changeCurrentDeviceIndex(short); // Change current device index
private:
    void updateDeviceList();    // Updates the device list

    cv::Mat frame;              // Contains the last image from capture device
    cv::VideoCapture capture;   // Current capture device

    short curDeviceIndex;           // Stores index of current selected capture device
    QList<Camera> captureDevices;   // Stores all available capture devices
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CaptureManager::Flags);

#endif // CAPTUREMANAGER_H
