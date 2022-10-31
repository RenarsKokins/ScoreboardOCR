#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "recognitionmanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    void updateFieldsWithValues();
    void addRecognitionManager(RecognitionManager *);

    int getSvmSize();
    QString getPath();
    int getNoiseIgnoreSize();
    float getNoiseIgnoreRatio();

private slots:
    void showFileExplorerAndUpdatePath();

private:
    RecognitionManager *recManager;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
