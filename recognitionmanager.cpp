#include <QDebug>
#include "recognitionmanager.h"

RecognitionManager::RecognitionManager()
{
    qDebug() << "Initializing recognition manager...";
}

RecognitionManager::~RecognitionManager()
{
    qDebug() << "Destroying recognition manager...";
}

Selection *RecognitionManager::addSelection(QString title, SelectionType type)
{
    Selection sel;
    sel.setName(title);
    sel.setType(type);
    selections.append(sel);
    flags.setFlag(RecognitionManager::selectingSelection, true);
    return &selections.last();
}

void RecognitionManager::deleteSelection(unsigned short i)
{
    selections.removeAt(i);
    qDebug() << "Deleted a selection in recognition manager!";
    flags.setFlag(RecognitionManager::selectingSelection, false);
}

void RecognitionManager::deleteSelection(Selection *sel)
{
    // Loop through all selections and delete if matches passed pointer
    int i = 0;
    for(Selection &selection : selections)
    {
        if(&selection == sel)
        {
            selections.removeAt(i);
            qDebug() << "Deleted a selection in recognition manager!";
            break;
        }
        i++;
    }
    flags.setFlag(RecognitionManager::selectingSelection, false);
}

void RecognitionManager::updateSelectionCoordinates(QRect rect)
{
    selections.last().setSelectionPos(rect.x(), rect.y(), rect.width(), rect.height());
    flags.setFlag(RecognitionManager::selectingSelection, false);
}

void RecognitionManager::findNumbers(cv::Mat *img)
{
    QRect qRect;
    cv::Rect rect;
    cv::Mat number;
    cv::Mat croppedImg;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;

    for(Selection &selection : selections)
    {
        qRect = selection.getSelectionPos();
        selection.clearNumbers();
        croppedImg = cv::Mat(*img, cv::Rect(qRect.x(),
                                            qRect.y(),
                                            qRect.width(),
                                            qRect.height()));
        cv::findContours(croppedImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for(std::vector<cv::Point> &contour : contours)
        {
            rect = cv::boundingRect(contour);
            // filter out non-numbers (numbers are usually taller than size of width + not super small)
            if((rect.height < rect.width) || (rect.height < noiseIgnoreSize) || (rect.height < (qRect.width() * 0.5)))
                continue;
            number = cv::Mat(croppedImg, rect).clone();

            // Scale image so it has a height of svmSize, but width proportional to original bounding rect.
            // That means that if original bounding rect width is 50 and height 100, new size will be:
            // width: 12 height: 24
            int width = ((float)number.cols / number.rows) * svmSize;
            cv::resize(number, number, cv::Size(width, svmSize), 0, 0);
            // Now we add borders to that scaled down image, so that its size will be svmSize * svmSize
            int left, right;
            if(width % 2)
            {
                // odd
                left = (svmSize - width) / 2;
                right = left + 1;
            } else
            {
                // even
                left = (svmSize - width) / 2;
                right = left;
            }
            cv::copyMakeBorder(number, number, 0, 0, left, right, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
            selection.addNumber(number);
        }
    }
}

bool RecognitionManager::selectionsAdded()
{
    return !selections.isEmpty();
}
