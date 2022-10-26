#ifndef RECOGNITIONMANAGER_H
#define RECOGNITIONMANAGER_H

#include <QList>
#include <QString>
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

    void deleteSelection(Selection*);
    void deleteSelection(unsigned short);
    void updateSelectionCoordinates(QRect);
    Selection *addSelection(QString, SelectionType);

private:
    QList<Selection> selections;
};

#endif // RECOGNITIONMANAGER_H
