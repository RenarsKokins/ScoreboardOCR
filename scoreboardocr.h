#ifndef SCOREBOARDOCR_H
#define SCOREBOARDOCR_H

#include <QThread>
#include <QComboBox>
#include <QMainWindow>
#include <QCloseEvent>
#include <QGraphicsScene>
#include "mainworker.h"
#include "capturemanager.h"

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
    void displayMainImage(cv::Mat *);   // Display main image in GUI
    void displaySmallImage(cv::Mat *);  // Display small image in GUI

signals:
    void startMainWorker();             // Starts main worker thread

private:
    Ui::ScoreboardOCR *ui;              // Pointer to main UI
    QThread workerThread;               // Place where MainWorker operates
    MainWorker *mainWorker;             // Thread where all processes happen (capture, filters, etc.)
    CaptureManager *capManager;         // Main capture manager
    QGraphicsScene *mainGraphicsScene;  // Main graphics scene
    QGraphicsScene *smallGraphicsScene; // Small graphics scene
    QGraphicsPixmapItem *mainPixmap;
    QGraphicsPixmapItem *smallPixmap;

    QPixmap mat2pix(cv::Mat *img);      // Convert mat image to pixmap

    // UI updates
    int updateDeviceDropdown();
    void closeEvent(QCloseEvent *);
    void updateCaptureTab();
};
#endif // SCOREBOARDOCR_H
