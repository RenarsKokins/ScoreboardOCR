#include <QDebug>
#include "outputmanager.h"

OutputManager::OutputManager()
{

}

QString OutputManager::getOutputPath()
{
    return outputPath;
}

void OutputManager::setOutputPath(QString path)
{
    outputPath = path;
}

QString OutputManager::getOutputFilename()
{
    return outputFilename;
}

void OutputManager::setOutputFilename(QString name)
{
    outputFilename = name;
}
