#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include <QWidget>
#include "selection.h"

namespace Ui {
class SelectionWidget;
}

class SelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectionWidget(QWidget *parent = nullptr);
    ~SelectionWidget();
    void updateSelection();
    void addSelection(Selection*);
    Selection *getSelection();

private:
    Selection *sel;
    Ui::SelectionWidget *ui;
};

#endif // SELECTIONWIDGET_H
