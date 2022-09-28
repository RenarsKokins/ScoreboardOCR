#ifndef SCOREBOARDOCR_H
#define SCOREBOARDOCR_H

#include <QMainWindow>
#include <QComboBox>
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
    void setCurrentDevice(int val);

private:
    Ui::ScoreboardOCR *ui;
    CaptureManager *capManager;     // Main capture manager

    // UI updates
    int updateDeviceDropdown();
};
#endif // SCOREBOARDOCR_H
