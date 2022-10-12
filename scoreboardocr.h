#ifndef SCOREBOARDOCR_H
#define SCOREBOARDOCR_H

#include <QThread>
#include <QComboBox>
#include <QMainWindow>
#include <QCloseEvent>
#include <QGraphicsScene>
#include "mainworker.h"
#include "capturescene.h"
#include "capturemanager.h"
#include "maincapturescene.h"

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
    void setCurrentDevice(int val);     // Combobox device select slot
    void updateEdges(QList<QPoint>);    // Update edges for image transformation
    void displayMainImage(cv::Mat *);   // Display main image in GUI
    void displaySmallImage(cv::Mat *);  // Display small image in GUI

signals:
    void startMainWorker();             // Starts main worker thread

private:
    Ui::ScoreboardOCR *ui;              // Pointer to main UI
    QThread workerThread;               // Place where MainWorker operates
    MainWorker *mainWorker;             // Thread where all processes happen (capture, filters, etc.)
    CaptureManager *capManager;         // Main capture manager
    CaptureScene *smallGraphicsScene;   // Small graphics scene
    MainCaptureScene *mainGraphicsScene;// Main graphics scene

    // UI updates
    void updateCaptureTab();
    int updateDeviceDropdown();
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
};
#endif // SCOREBOARDOCR_H
