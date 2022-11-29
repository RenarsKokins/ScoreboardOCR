#include <QDebug>
#include <QMessageBox>
#include <QDialogButtonBox>
#include "selectiondialog.h"
#include "ui_selectiondialog.h"

SelectionDialog::SelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectionDialog)
{
    ui->setupUi(this);
    ui->selectionTypeComboBox->addItems(selectionType.getTypeDescriptions());   // Add descriptions to combobox
}

SelectionDialog::~SelectionDialog()
{
    delete ui;
}

void SelectionDialog::accept()
{
    if(ui->selectionName->text().isEmpty())
    {
        QMessageBox::information(this, tr("Info"), tr("You must specify a name for selection!"), QMessageBox::Ok);
        return;
    }

    // Check uniqueness of our new word
    for(QString &uName : uniqueNames)
    {
        if(uName != ui->selectionName->text())
            continue;
        QMessageBox::information(this, tr("Info"), tr("Please enter a unique name for selection!"), QMessageBox::Ok);
        return;
    }

    if(ui->selectionTypeComboBox->currentIndex() < 0)
    {
        QMessageBox::information(this, tr("Info"), tr("You must select a type!"), QMessageBox::Ok);
        return;
    }
    selectionName = ui->selectionName->text();
    selectionType.setType(ui->selectionTypeComboBox->currentIndex());
    done(1);
}

void SelectionDialog::reject()
{
    done(0);
}

QString SelectionDialog::getName()
{
    return selectionName;
}

SelectionType SelectionDialog::getType()
{
    return selectionType;
}

void SelectionDialog::setUniqueNames(QList<QString> names)
{
    uniqueNames.clear();
    uniqueNames.append(names);
}
