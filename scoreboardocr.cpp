#include "scoreboardocr.h"
#include "ui_scoreboardocr.h"

ScoreboardOCR::ScoreboardOCR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreboardOCR)
{
    ui->setupUi(this);
    // Initialize managers
    capManager = new CaptureManager();

    // Update device combobox
    updateDeviceDropdown();

    // Connect signals/slots
    connect(ui->captureDeviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDevice(int)));
}

ScoreboardOCR::~ScoreboardOCR()
{
    delete ui;
    delete capManager;
}

int ScoreboardOCR::updateDeviceDropdown()
{
    for (Camera &cam : capManager->getDevices())
    {
        ui->captureDeviceComboBox->addItem(cam.name + " | " + cam.description);
    }
    return 1;
}

void ScoreboardOCR::setCurrentDevice(int val)
{
    capManager->changeCurrentDeviceIndex(val);
}
