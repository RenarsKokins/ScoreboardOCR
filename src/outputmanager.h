#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <QList>
#include <QString>
#include <QStringList>
#include "selection.h"

class OutputManager
{
public:
    enum OutputFormat
    {
        JSON,
        XML,
        CSV,
        TEXT,
    };

    OutputManager();

    QString getOutputPath();
    void setOutputPath(QString);
    QString getOutputFilename();
    void saveRecognizedNumbers();           // Do raw number processing and save to file
    OutputFormat getOutputFormat();
    void setOutputFilename(QString);
    void setOutputFormat(OutputFormat);
    QStringList getFormatDescriptions();
    void addSelections(QList<Selection>*);

private:
    QString outputPath = ".";
    QList<Selection> *selections;
    QString outputFilename = "scoreboardocr";

    OutputFormat format = OutputFormat::JSON;
    QStringList formatDescriptions = {
        "JSON file (.json)",
        "XML file (.xml)",
        "CSV file (.csv)",
        "Multiple text files (.txt)",
    };

    QString serializeRawNumbers(QVector<int>, SelectionType);
};

#endif // OUTPUTMANAGER_H
