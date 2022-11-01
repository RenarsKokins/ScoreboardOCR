#include "selectionwidget.h"
#include "ui_selectionwidget.h"

SelectionWidget::SelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectionWidget)
{
    ui->setupUi(this);
}

SelectionWidget::~SelectionWidget()
{
    delete ui;
}

void SelectionWidget::updateSelection()
{
    ui->selectionName->setText(sel->getName());
    ui->selectionDescription->setText(sel->getType().getDescription());
}

void SelectionWidget::addSelection(Selection *sel)
{
    this->sel = sel;
    updateSelection();
}

Selection *SelectionWidget::getSelection()
{
    return sel;
}
