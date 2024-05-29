#ifndef SCOREBOARDOCR_H
#define SCOREBOARDOCR_H

#include <QThread>
#include <QComboBox>
#include <QMainWindow>
#include <QCloseEvent>
#include <QGraphicsScene>
#include "mainworker.h"
#include "capturescene.h"
#include "filtermanager.h"
#include "outputmanager.h"
#include "capturemanager.h"
#include "settingsmanager.h"
#include "maincapturescene.h"
#include "recognitionmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ScoreboardOCR; }
QT_END_NAMESPACE

class ScoreboardOCR : public QMainWindow
{
    Q_OBJECT

public:
    ScoreboardOCR(QWidget *parent = nullptr);
    ~ScoreboardOCR();

public slots:
    void doEdges();                     // edges button slot
    void doCapture();                   // Capture button slot
    void openSettings();                // Display settings dialog
    void addSelection();                // Display selection dialog
    void changeGaps(int);               // Change threshold value in FilterManager
    void changeBlur(int);               // Change blur value in FilterManager
    void setOutputPath();               // Let user choose a path and save it
    void changeErode(int);              // Change erode value in FilterManager
    void changeItalic(int);             // Change italic value in FilterManager
    void deleteSelection();             // Delete currently selected selection
    void changeInvert(bool);            // Invert colors on image (after thresholding)
    void changeDialate(int);            // Change dialate value in FilterManager
    void setOutputFilename();           // Set output filename from lineedit
    void changeThreshold(int);          // Change threshold value in FilterManager
    void setOutputFormat(int);          // Set output format
    void updateDeviceDropdown();        // Update UI device combobox
    void setCurrentDevice(int);         // Combobox device select slot
    void setCaptureType(int);           // Capture type tab slot
    void setCameraIPAddress(QString);   // Set camera IP address
    void updateEdges(QList<QPoint>);    // Update edges for image transformation
    void displayMainImage(cv::Mat *);   // Display main image in GUI
    void displaySmallImage(cv::Mat *);  // Display small image in GUI
    void updateSelectionCoordinates(QRect); // Update selection rectangle coords


signals:
    void startMainWorker();             // Starts main worker thread

private:
    Ui::ScoreboardOCR *ui;              // Pointer to main UI
    QThread workerThread;               // Place where MainWorker operates
    MainWorker *mainWorker;             // Thread where all processes happen (capture, filters, etc.)
    FilterManager *filManager;          // Filter manager (applies transforms, thresholds, etc.)
    OutputManager *outManager;          // Pointer to output manager
    CaptureManager *capManager;         // Main capture manager
    RecognitionManager *recManager;     // Recognition manager (detects numbers, defines boxes, etc.)
    SettingsManager *settingsManager;   // Settings manager
    CaptureScene *smallGraphicsScene;   // Small graphics scene
    MainCaptureScene *mainGraphicsScene;// Main graphics scene

    // UI updates
    void updateOutputTab();
    void updateCaptureTab();
    void updateRecognitionTab();
    void addFormatsToOutputTab();
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
};
#endif // SCOREBOARDOCR_H
