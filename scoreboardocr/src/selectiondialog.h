#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H

#include <QList>
#include <QDialog>
#include <QString>
#include "selectiontype.h"

namespace Ui {
class SelectionDialog;
}

class SelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(QWidget *parent = nullptr);
    ~SelectionDialog();
    QString getName();                      // Returns name of selection
    SelectionType getType();                // Returns selected type for selection
    void setUniqueNames(QList<QString>);    // Used to set already used names for selections, because they should be unique

private slots:
    void accept() override;
    void reject() override;

private:
    Ui::SelectionDialog *ui;
    QString selectionName = "";
    SelectionType selectionType;
    QList<QString> uniqueNames;
};

#endif // SELECTIONDIALOG_H
