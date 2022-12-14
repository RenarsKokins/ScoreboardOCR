#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QWidget>
#include "mainworker.h"
#include "outputmanager.h"
#include "settingsdialog.h"
#include "recognitionmanager.h"

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    SettingsManager();
    ~SettingsManager();
    void loadSettings();                                // Load settings into manangers
    void addMainWorker(MainWorker *);
    void saveSettings(SettingsDialog *);                // Save settings from managers
    void updateSettings(SettingsDialog *);              // Update settings to managers
    void addOutputManager(OutputManager *);
    void showSettingsDialog(QWidget *parent);           // Show a settings dialog which might change settings
    void addRecognitionManager(RecognitionManager *);

private:
    MainWorker *worker;
    OutputManager *outManager;
    RecognitionManager *recManager;
    std::string configPath = "settings.csv";
};

#endif // SETTINGSMANAGER_H
