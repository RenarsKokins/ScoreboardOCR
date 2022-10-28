#ifndef SELECTION_H
#define SELECTION_H

#include <QRect>
#include <QVector>
#include <opencv2/core.hpp>
#include "selectiontype.h"

class Selection
{
public:
    Selection();
    QString getName();
    void clearNumbers();            // Clears numbers vector
    void setName(QString);
    void addNumber(cv::Mat);        // Add a number to numbers vector
    SelectionType getType();
    QRect getSelectionPos();
    void setType(SelectionType);
    QVector<cv::Mat> *getNumbers(); // Returns a pointer to numbers vector
    void setSelectionPos(int, int, int, int);

private:
    QRect rect;                 // Rect object for saving coordinates
    QString name;               // Name of selection
    SelectionType type;         // Type of selection
    QVector<cv::Mat> numbers;   // A vector where images are stored for classification
};

#endif // SELECTION_H
