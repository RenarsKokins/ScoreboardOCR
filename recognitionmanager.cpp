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
