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

QStringList OutputManager::getFormatDescriptions()
{
    return formatDescriptions;
}

void OutputManager::setOutputFormat(OutputManager::OutputFormat format)
{
    this->format = format;
}

OutputManager::OutputFormat OutputManager::getOutputFormat()
{
    return format;
}

void OutputManager::addSelections(QList<Selection> *sel)
{
    selections = sel;
}

void OutputManager::saveRecognizedNumbers()
{
    QString serializedNumbers;
    for(Selection &selection : *selections)
    {
        serializedNumbers = serializeRawNumbers(*selection.getRawRecognitionNumbers(),
                                                selection.getType());
        qDebug() << serializedNumbers;
    }
}

QString OutputManager::serializeRawNumbers(QVector<int> numbers, SelectionType type)
{
    QString serialized = "";
    if(numbers.count() < 1)
        return serialized;

    switch(type.getType())
    {
    case SelectionType::anyNumber:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        break;

    case SelectionType::longPeriod:
        switch(numbers[0])
        {
        case 1:
            serialized = "1st";
            break;
        case 2:
            serialized = "2nd";
            break;
        case 3:
            serialized = "3rd";
            break;
        case 4:
            serialized = "4th";
            break;
        case 5:
            serialized = "5th";
            break;
        case 6:
            serialized = "6th";
            break;
        case 7:
            serialized = "7th";
            break;
        case 8:
            serialized = "8th";
            break;
        case 9:
            serialized = "9th";
            break;
        }
        break;

    case SelectionType::period:
        switch(numbers[0])
        {
        case 1:
            serialized = "1st";
            break;
        case 2:
            serialized = "2nd";
            break;
        case 3:
            serialized = "3rd";
            break;
        case 4:
            serialized = "4th";
            break;
        default:
            serialized = "OT";
            break;
        }
        break;

    case SelectionType::seconds:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        break;

    case SelectionType::secondsMilliseconds:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        if(serialized.count() == 1)
            serialized.insert(0, "0.");
        else if(serialized.count() == 2)
            serialized.insert(1, ".");
        else if(serialized.count() == 3)
            serialized.insert(2, ".");
        else if(serialized.count() == 4)
            serialized.insert(3, ".");
        break;

    case SelectionType::time:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        if(serialized.count() == 1)
            serialized.insert(0, "00:0");
        else if(serialized.count() == 2)
            serialized.insert(0, "00:");
        else if(serialized.count() == 3)
            serialized.insert(1, ":");
        else if(serialized.count() == 4)
            serialized.insert(2, ":");
        else if(serialized.count() == 5)
            serialized.insert(serialized.count() - 2, ":");
        else if(serialized.count() > 5)
        {
            serialized.insert(serialized.count() - 2, ":");
            serialized.insert(serialized.count() - 5, ":");
        }
        break;

    case SelectionType::timeMilliseconds:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        if(serialized.count() == 1)
            serialized.insert(0, "0.");
        else if(serialized.count() == 2)
            serialized.insert(1, ".");
        else if(serialized.count() == 3)
            serialized.insert(2, ".");
        else if(serialized.count() == 4)
            serialized.insert(2, ":");
        else if(serialized.count() > 4)
            serialized.insert(serialized.count() - 2, ":");
        break;
    }
    return serialized;
}
