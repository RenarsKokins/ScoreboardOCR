#ifndef RECOGNITIONMANAGER_H
#define RECOGNITIONMANAGER_H

#include <QList>
#include <vector>
#include <QVector>
#include <QString>
#include <opencv2/ml.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "selection.h"
#include "selectiontype.h"

class RecognitionManager
{
public:
    enum Flag
    {
        svmLoaded = 1,
        saveNumbers = 2,
        movingSelection = 4,
        selectingSelection = 8,
    };
    Q_DECLARE_FLAGS(Flags, Flag);
    Flags flags;

    RecognitionManager();
    ~RecognitionManager();

    void loadSVM();                                     // Loads a SVM model from a certain path
    bool selectionsAdded();
    void recognizeNumbers();                            // Recognizes (using SVM) numbers in image
    void findNumbers(cv::Mat *);                        // Uses defined selections to find numbers in image
    void saveNumbersAsFiles(QString);                   // Save recognized numbers for debug/training purposes
    void deleteSelection(Selection *);
    void deleteSelection(unsigned short);
    void updateSelectionCoordinates(QRect);
    Selection *addSelection(QString, SelectionType);

    int svmSize = 16;               // Size of SVM input (used for number image resizing to a specified size)
    int noiseIgnoreSize = 5;        // Size under which selection is considered noise (in findNumbers())
    QString svmPath = "SVM.svm";    // A path to SVM serialized data
    float noiseIgnoreRatio = 0.5;   // Size which is used to filter noise. If detected contours' height is proportionally smaller than selection, ignores it
    QString imgSavePath = "images/";// A path where to save iamges

private:
    cv::Ptr<cv::ml::SVM> svm;       // Pointer to our source vector machine (created in constructor)
    QList<Selection> selections;

    static bool compareRect(const cv::Rect &, const cv::Rect &);   // Compare two rects so that we can sort them
};

#endif // RECOGNITIONMANAGER_H
