#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "QWidget"
#include "recognitionmanager.h"

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();
    void loadSettings();                        // Load settings into manangers
    void saveSettings();                        // Save settings from manangers
    void showSettingsDialog(QWidget *parent);   // Show a settings dialog which might change settings
    void addRecognitionManager(RecognitionManager*);

private:
    RecognitionManager *recManager;
    std::string configPath = "settings.csv";
};

#endif // SETTINGSMANAGER_H
