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
    void changeErode(int);              // Change erode value in FilterManager
    void changeItalic(int);             // Change italic value in FilterManager
    void deleteSelection();             // Delete currently selected selection
    void changeDialate(int);            // Change dialate value in FilterManager
    void changeThreshold(int);          // Change threshold value in FilterManager
    void setCurrentDevice(int val);     // Combobox device select slot
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
    CaptureManager *capManager;         // Main capture manager
    RecognitionManager *recManager;     // Recognition manager (detects numbers, defines boxes, etc.)
    SettingsManager *settingsManager;   // Settings manager
    CaptureScene *smallGraphicsScene;   // Small graphics scene
    MainCaptureScene *mainGraphicsScene;// Main graphics scene

    // UI updates
    void updateCaptureTab();
    int updateDeviceDropdown();
    void updateRecognitionTab();
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
};
#endif // SCOREBOARDOCR_H
