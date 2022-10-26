#ifndef SELECTION_H
#define SELECTION_H

#include <QPen>
#include <QRect>
#include <QBrush>
#include "selectiontype.h"

class Selection
{
public:
    Selection();
    QString getName();
    SelectionType getType();
    QRect getSelectionPos();
    void setName(QString);
    void setType(SelectionType);
    void setSelectionPos(int, int, int, int);

private:
    QRect rect;             // Rect object for saving coordinates
    QString name;           // Name of selection
    SelectionType type;     // Type of selection
};

#endif // SELECTION_H
