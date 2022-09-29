#ifndef SCOREBOARDOCR_H
#define SCOREBOARDOCR_H

#include <QMainWindow>
#include <QComboBox>
#include "capturemanager.h"
#include "displaymanager.h"

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
    void setCurrentDevice(int val); // Combobox device select slot
    void startCapture();            // Start capture button slot
    void stopCapture();            // Stop capture button slot

private:
    Ui::ScoreboardOCR *ui;
    CaptureManager *capManager;     // Main capture manager
    DisplayManager *disManager;     // Main display manager

    // UI updates
    int updateDeviceDropdown();
    void updateCaptureTab();
};
#endif // SCOREBOARDOCR_H
