#ifndef RECOGNITIONMANAGER_H
#define RECOGNITIONMANAGER_H

#include <QList>
#include <vector>
#include <QVector>
#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "selection.h"
#include "selectiontype.h"

class RecognitionManager
{
public:
    enum Flag
    {
        movingSelection = 1,
        selectingSelection = 2,
    };
    Q_DECLARE_FLAGS(Flags, Flag);
    Flags flags;

    RecognitionManager();
    ~RecognitionManager();

    bool selectionsAdded();
    void findNumbers(cv::Mat *);                        // Uses defined selections to find numbers in image
    void recognizeNumbers(cv::Mat *);                   // Recognizes (using SVM) numbers in image
    void deleteSelection(Selection *);
    void deleteSelection(unsigned short);
    void updateSelectionCoordinates(QRect);
    Selection *addSelection(QString, SelectionType);

private:
    int svmSize = 16;               // Size of SVM input (used for number image resizing to a specified size)
    int noiseIgnoreSize = 5;        // Size under which selection is considered noise (in findNumbers())
    QList<Selection> selections;
};

#endif // RECOGNITIONMANAGER_H
