#include <QDebug>
#include <fstream>
#include <sstream>
#include <QVector>
#include "settingsmanager.h"

SettingsManager::SettingsManager()
{
    qDebug() << "Initializing settings manager...";
}

SettingsManager::~SettingsManager()
{
    qDebug() << "Destroying settings manager...";
}

void SettingsManager::addRecognitionManager(RecognitionManager *rec)
{
    recManager = rec;
}

void SettingsManager::addOutputManager(OutputManager *out)
{
    outManager = out;
}

void SettingsManager::addMainWorker(MainWorker *m)
{
    worker = m;
}

void SettingsManager::updateSettings(SettingsDialog *dialog)
{
    recManager->svmPath = dialog->getPath();
    recManager->svmSize = dialog->getSvmSize();
    recManager->noiseIgnoreSize = dialog->getNoiseIgnoreSize();
    recManager->noiseIgnoreRatio = dialog->getNoiseIgnoreRatio();
    recManager->flags.setFlag(RecognitionManager::saveNumbers, dialog->getSaveNumbers());
    recManager->loadSVM();
    worker->fps = dialog->getFps();
}

void SettingsManager::showSettingsDialog(QWidget *parent)
{
    SettingsDialog dialog(parent);
    dialog.addMainWorker(worker);
    dialog.addRecognitionManager(recManager);
    dialog.updateFieldsWithValues();

    bool applied = 0;
    applied = dialog.exec();
    if(!applied)
        return;
    saveSettings(&dialog);
}

void SettingsManager::loadSettings()
{
    std::ifstream fin;
    std::string line, word;
    QVector<std::string> row;
    QVector<QVector<std::string>> settings;

    // Try to open config file
    fin.open(configPath);
    if(!fin.is_open())
    {
        qDebug() << "Settings file could not be open for reading!";
        return;
    }

    // Read the config file into a vector
    while(!fin.eof())
    {
        row.clear();
        std::getline(fin, line);
        std::stringstream s(line);
        while(std::getline(s, word, ','))
        {
            row.push_back(word);
        }
        if(row.size() > 1)
            settings.push_back(row);
    }
    fin.close();

    // Parse the config
    for(QVector<std::string> &setting : settings)
    {
        // RecognitionManager
        if(setting[0] == "svmPath")
            recManager->svmPath = QString::fromUtf8(setting[1].data(), setting[1].size());
        else if(setting[0] == "svmSize")
            recManager->svmSize = std::stoi(setting[1]);
        else if(setting[0] == "noiseIgnoreSize")
            recManager->noiseIgnoreSize = std::stoi(setting[1]);
        else if(setting[0] == "noiseIgnoreRatio")
            recManager->noiseIgnoreRatio = std::stof(setting[1]);
        else if(setting[0] == "fps")
            worker->fps = std::stoi(setting[1]);
        // OutputManager
        else if(setting[0] == "outputPath")
            outManager->setOutputPath(QString::fromUtf8(setting[1].data(), setting[1].size()));
        else if(setting[0] == "outputFilename")
            outManager->setOutputFilename(QString::fromUtf8(setting[1].data(), setting[1].size()));
        else if(setting[0] == "outputFormat")
            outManager->setOutputFormat(static_cast<OutputManager::OutputFormat>(std::stoi(setting[1])));
    }
}

void SettingsManager::saveSettings(SettingsDialog *dialog = nullptr)
{
    std::ofstream fout;

    if(dialog != nullptr)
        updateSettings(dialog);

    // Try to open config file
    fout.open(configPath);
    if(!fout.is_open())
    {
        qDebug() << "Settings file could not be open for writing!";
        return;
    }

    // RecognitionManager
    fout << "svmPath," << recManager->svmPath.toStdString() << std::endl;
    fout << "svmSize," << recManager->svmSize << std::endl;
    fout << "noiseIgnoreSize," << recManager->noiseIgnoreSize << std::endl;
    fout << "noiseIgnoreratio," << recManager->noiseIgnoreRatio << std::endl;
    fout << "fps," << worker->fps << std::endl;
    // OutputManager
    fout << "outputPath," << outManager->getOutputPath().toStdString() << std::endl;
    fout << "outputFilename," << outManager->getOutputFilename().toStdString() << std::endl;
    fout << "outputFormat," << outManager->getOutputFormat() << std::endl;
    fout.close();
}

