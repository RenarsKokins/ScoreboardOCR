#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "mainworker.h"
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
    void addMainWorker(MainWorker *);
    void addRecognitionManager(RecognitionManager *);

    int getFps();
    int getSvmSize();
    QString getPath();
    int getNoiseIgnoreSize();
    float getNoiseIgnoreRatio();

signals:
    void emitSave(SettingsDialog *);

private slots:
    void showFileExplorerAndUpdatePath();
    void doSave();

private:
    MainWorker *worker;
    Ui::SettingsDialog *ui;
    RecognitionManager *recManager;
};

#endif // SETTINGSDIALOG_H
