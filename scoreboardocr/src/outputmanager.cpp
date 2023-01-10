#include <QFile>
#include <QDebug>
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QXmlStreamWriter>
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
    QStringList selectionNames;
    QStringList serializedNumbers;
    for(Selection &selection : *selections)
    {
        QString serializedNumber = serializeRawNumbers(*selection.getRawRecognitionNumbers(),
                                                selection.getType());
        serializedNumbers.push_back(serializedNumber);
        selectionNames.push_back(selection.getName());
    }
    saveNumbersToFile(serializedNumbers, selectionNames);
}

void OutputManager::saveNumbersToFile(QStringList serializedNumbers, QStringList selectionNames)
{
    switch(format)
    {
    case OutputFormat::CSV:
        OutputManager::saveAsCSV(serializedNumbers, selectionNames);
        break;
    case OutputFormat::XML:
        OutputManager::saveAsXML(serializedNumbers, selectionNames);
        break;
    case OutputFormat::TEXT:
        OutputManager::saveAsText(serializedNumbers, selectionNames);
        break;
    case OutputFormat::JSON:
        OutputManager::saveAsJSON(serializedNumbers, selectionNames);
        break;
    }
}

void OutputManager::saveAsCSV(QStringList serializedNumbers, QStringList selectionNames)
{
    QString fullPath = outputPath + '/' + outputFilename + ".csv";
    std::ofstream fout;
    fout.open(fullPath.toStdString());
    if(!fout.is_open())
    {
        qDebug() << "Output file" << fullPath << "could not be open!";
        return;
    }

    for(int i = 0; i < serializedNumbers.length(); i++)
    {
        fout << selectionNames[i].toStdString() << "," << serializedNumbers[i].toStdString() << std::endl;
    }
    fout.close();
}

void OutputManager::saveAsXML(QStringList serializedNumbers, QStringList selectionNames)
{
    QString fullPath = outputPath + '/' + outputFilename + ".xml";
    QFile fout(fullPath);
    fout.open(QIODevice::WriteOnly);
    if(!fout.isOpen())
    {
        qDebug() << "Output file" << fullPath << "could not be open!";
        return;
    }

    QXmlStreamWriter stream(&fout);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("OCR");
    for(int i = 0; i < serializedNumbers.length(); i++)
    {
        stream.writeTextElement(selectionNames[i], serializedNumbers[i]);
    }
    stream.writeEndDocument();

    fout.close();
}

void OutputManager::saveAsJSON(QStringList serializedNumbers, QStringList selectionNames)
{
    QJsonObject object;
    for(int i = 0; i < serializedNumbers.length(); i++)
    {
        object.insert(selectionNames[i], serializedNumbers[i]);
    }

    QString fullPath = outputPath + '/' + outputFilename + ".json";
    QFile fout(fullPath);
    fout.open(QIODevice::WriteOnly);
    if(!fout.isOpen())
    {
        qDebug() << "Output file" << fullPath << "could not be open!";
        return;
    }
    QJsonDocument stream(object);
    fout.write(stream.toJson());
    fout.close();
}

void OutputManager::saveAsText(QStringList serializedNumbers, QStringList selectionNames)
{
    std::ofstream fout;
    for(int i = 0; i < serializedNumbers.length(); i++)
    {
        fout.open(outputPath.toStdString() + '/' + selectionNames[i].toStdString() + ".txt");
        if(!fout.is_open())
        {
            qDebug() << "Output file (.txt) could not be open!";
            continue;
        }
        fout << serializedNumbers[i].toStdString() << std::endl;
        fout.close();
    }
}

QString OutputManager::serializeRawNumbers(QVector<int> numbers, SelectionType type)
{
    QString serialized = "";
    if(numbers.length() < 1)
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
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        switch(numbers[numbers.length() - 1])
        {
        case 1:
            serialized.append("st");
            break;
        case 2:
            serialized.append("nd");
            break;
        case 3:
            serialized.append("rd");
            break;
        default:
            serialized.append("th");
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
        if(serialized.length() == 1)
            serialized.insert(0, "0.");
        else if(serialized.length() == 2)
            serialized.insert(1, ".");
        else if(serialized.length() == 3)
            serialized.insert(2, ".");
        else if(serialized.length() == 4)
            serialized.insert(3, ".");
        break;

    case SelectionType::time:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        if(serialized.length() == 1)
            serialized.insert(0, "00:0");
        else if(serialized.length() == 2)
            serialized.insert(0, "00:");
        else if(serialized.length() == 3)
            serialized.insert(1, ":");
        else if(serialized.length() == 4)
            serialized.insert(2, ":");
        else if(serialized.length() == 5)
            serialized.insert(serialized.length() - 2, ":");
        else if(serialized.length() > 5)
        {
            serialized.insert(serialized.length() - 2, ":");
            serialized.insert(serialized.length() - 5, ":");
        }
        break;

    case SelectionType::timeMilliseconds:
        for(int &number : numbers)
        {
            serialized.append(QString::number(number));
        }
        if(serialized.length() == 1)
            serialized.insert(0, "0.");
        else if(serialized.length() == 2)
            serialized.insert(1, ".");
        else if(serialized.length() == 3)
            serialized.insert(2, ".");
        else if(serialized.length() == 4)
            serialized.insert(2, ":");
        else if(serialized.length() == 5)
            serialized.insert(serialized.length() - 2, ":");
        else if(serialized.length() > 5)
        {
            serialized.insert(serialized.length() - 2, ":");
            serialized.insert(serialized.length() - 5, ":");
        }
        break;
    }
    return serialized;
}
