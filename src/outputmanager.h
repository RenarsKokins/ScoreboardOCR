#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <QString>

class OutputManager
{
public:
    OutputManager();

    QString getOutputPath();
    void setOutputPath(QString);
    QString getOutputFilename();
    void setOutputFilename(QString);

private:
    QString outputPath = ".";
    QString outputFilename = "scoreboardocr";
};

#endif // OUTPUTMANAGER_H
