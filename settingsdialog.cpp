#include <QString>
#include <QFileDialog>
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->pathSelectButton, SIGNAL(released()), this, SLOT(showFileExplorerAndUpdatePath()));
}

void SettingsDialog::updateFieldsWithValues()
{
    ui->svmPathTextbox->setText(recManager->svmPath);
    ui->svmSizeSpinbox->setValue(recManager->svmSize);
    ui->noiseIgnoreSizeSpinbox->setValue(recManager->noiseIgnoreSize);
    ui->noiseIgnoreRatioSpinbox->setValue(recManager->noiseIgnoreRatio);
}

void SettingsDialog::addRecognitionManager(RecognitionManager *rec)
{
    recManager = rec;
}

void SettingsDialog::showFileExplorerAndUpdatePath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open SVM file"), tr("."), tr("SVM file (*.svm)"));
    if(!path.isEmpty())
        ui->svmPathTextbox->setText(path);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::getSvmSize()
{
   return ui->svmSizeSpinbox->value();
}

QString SettingsDialog::getPath()
{
   return ui->svmPathTextbox->text();
}

int SettingsDialog::getNoiseIgnoreSize()
{
   return ui->noiseIgnoreSizeSpinbox->value();
}

float SettingsDialog::getNoiseIgnoreRatio()
{
   return ui->noiseIgnoreRatioSpinbox->value();
}
