#include <QMessageBox>
#include "scoreboardocr.h"
#include "ui_scoreboardocr.h"

ScoreboardOCR::ScoreboardOCR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreboardOCR)
{
    ui->setupUi(this);
    // Initialize managers
    capManager = new CaptureManager();
    disManager = new DisplayManager(ui->smallImage, ui->mainImage);

    disManager->addCaptureManager(capManager);

    // Update device combobox
    updateDeviceDropdown();

    // Connect signals/slots
    connect(ui->captureDeviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDevice(int)));  // Combobox device select
    connect(ui->startCaptureButton, SIGNAL(released()), this, SLOT(startCapture()));                          // Start capture button
    connect(ui->stopCaptureButton, SIGNAL(released()), this, SLOT(stopCapture()));                            // Stop capture button
    connect(ui->tempCapture, SIGNAL(released()), this, SLOT(captureOneFrameAndShow()));
}

ScoreboardOCR::~ScoreboardOCR()
{
    delete ui;
    delete capManager;
    delete disManager;
}

int ScoreboardOCR::updateDeviceDropdown()
{
    for (Camera &cam : capManager->getDevices())
    {
        ui->captureDeviceComboBox->addItem(cam.name + " | " + cam.description);
    }
    return 1;
}

void ScoreboardOCR::updateCaptureTab()
{
    if (capManager->flags.testFlag(CaptureManager::captureSelected))
    {
        ui->startCaptureButton->setEnabled(true);
        ui->stopCaptureButton->setEnabled(false);
    } else
    {
        ui->startCaptureButton->setEnabled(false);
        ui->stopCaptureButton->setEnabled(false);
        ui->clearEdgesButton->setEnabled(false);
        ui->markEdgesButton->setEnabled(false);
        return;
    }

    if (capManager->flags.testFlag(CaptureManager::captureStarted))
    {
        ui->stopCaptureButton->setEnabled(true);
        ui->startCaptureButton->setEnabled(false);
    } else
    {
        ui->stopCaptureButton->setEnabled(false);
        ui->startCaptureButton->setEnabled(true);
        ui->clearEdgesButton->setEnabled(false);
        ui->markEdgesButton->setEnabled(false);
        return;
    }

    if (capManager->flags.testFlag(CaptureManager::edgesMarked))
    {
        ui->markEdgesButton->setEnabled(false);
        ui->clearEdgesButton->setEnabled(true);
    } else
    {
        ui->markEdgesButton->setEnabled(true);
        ui->clearEdgesButton->setEnabled(false);
    }
}

void ScoreboardOCR::setCurrentDevice(int val)
{
    capManager->changeCurrentDeviceIndex(val);
    updateCaptureTab();
}

void ScoreboardOCR::startCapture()
{
    switch(capManager->initCapture())
    {
    case -1:
        QMessageBox::warning(this, tr("Warning"), tr("No capture device selected!"), QMessageBox::Close);
        break;
    case 0:
        QMessageBox::critical(this, tr("Error"), tr("Cannot open capture device! Make sure it is working and plugged in!"), QMessageBox::Close);
        break;
    case 1:
        capManager->captureFrame();
    }
    updateCaptureTab();
    disManager->displaySmallPicture();
}

void ScoreboardOCR::stopCapture()
{
    if(!capManager->stopCapture())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No capture device ahs been opened so capture has already been stopped!"), QMessageBox::Close);
    }
    updateCaptureTab();
}

void ScoreboardOCR::closeEvent(QCloseEvent* event)
{
    int ret = QMessageBox::warning(this, tr("Warning"), tr("Are you sure you want to close this application?"), QMessageBox::No | QMessageBox::Yes);
    switch(ret)
    {
    case QMessageBox::Yes:
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    }
}

void ScoreboardOCR::captureOneFrameAndShow()
{
    capManager->captureFrame();
    disManager->displaySmallPicture();
}
