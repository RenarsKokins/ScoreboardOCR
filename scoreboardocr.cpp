#include <QDebug>
#include <QMessageBox>
#include "scoreboardocr.h"
#include "ui_scoreboardocr.h"

ScoreboardOCR::ScoreboardOCR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreboardOCR)
{
    ui->setupUi(this);
    mainGraphicsScene = new QGraphicsScene(this);
    smallGraphicsScene = new QGraphicsScene(this);
    ui->mainImage->setScene(mainGraphicsScene);
    ui->smallImage->setScene(smallGraphicsScene);

    // Initialize managers
    capManager = new CaptureManager();

    // Initialize main worker thread
    mainWorker = new MainWorker();
    mainWorker->moveToThread(&workerThread);
    mainWorker->addCaptureManager(capManager);
    connect(this, SIGNAL(startMainWorker()), mainWorker, SLOT(doWork()));
    connect(mainWorker, SIGNAL(setMainImage(cv::Mat*)), this, SLOT(displayMainImage(cv::Mat*)));
    connect(mainWorker, SIGNAL(setSmallImage(cv::Mat*)), this, SLOT(displaySmallImage(cv::Mat*)));

    // Update device combobox
    updateDeviceDropdown();

    // Connect UI signals/slots
    connect(ui->captureDeviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDevice(int)));  // Combobox device select
    connect(ui->captureButton, SIGNAL(released()), this, SLOT(doCapture()));                                  // Capture button pressed
    connect(ui->edgesButton, SIGNAL(released()), this, SLOT(doEdges()));                                      // Edges button pressed

    //connect(ui->startCaptureButton, SIGNAL(released()), this, SLOT(startCapture()));                          // Start capture button
    //connect(ui->stopCaptureButton, SIGNAL(released()), this, SLOT(stopCapture()));                            // Stop capture button
    //connect(ui->markEdgesButton, SIGNAL(released()), this, SLOT(startMarkingEdges()));                        // Start marking edges
    //connect(ui->clearEdgesButton, SIGNAL(released()), this, SLOT(clearEdges()));                              // Clear marked edges
}

ScoreboardOCR::~ScoreboardOCR()
{
    // Stop the thread if it is running
    if(workerThread.isRunning())
    {
        workerThread.requestInterruption();
        workerThread.quit();
        workerThread.wait();
    }
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

void ScoreboardOCR::updateCaptureTab()
{
    if (capManager->flags.testFlag(CaptureManager::captureSelected))
    {
        ui->captureButton->setEnabled(true);
        ui->captureButton->setText("Start capture");
    } else
    {
        ui->captureButton->setEnabled(false);
        ui->edgesButton->setEnabled(false);
        return;
    }

    if (capManager->flags.testFlag(CaptureManager::captureStarted))
    {
        ui->captureButton->setText("Stop capture");
        ui->edgesButton->setEnabled(true);
    } else
    {
        ui->captureButton->setText("Start capture");
        ui->edgesButton->setEnabled(false);
        return;
    }

    if (capManager->flags.testFlag(CaptureManager::edgesMarked))
    {
        ui->edgesButton->setText("Clear edges");
    } else
    {
        ui->edgesButton->setText("Mark edges");
    }
}

void ScoreboardOCR::setCurrentDevice(int val)
{
    capManager->changeCurrentDeviceIndex(val);
    updateCaptureTab();
}

void ScoreboardOCR::doCapture()
{
    if(!capManager->flags.testFlag(CaptureManager::captureStarted))
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
            workerThread.start();
            emit startMainWorker();
        }
    } else
    {
        if(!capManager->stopCapture())
            QMessageBox::warning(this, tr("Warning"), tr("No capture device has been opened so capture has already been stopped!"), QMessageBox::Close);
        if(workerThread.isRunning())
        {
            workerThread.requestInterruption();
            workerThread.quit();
            workerThread.wait();
        }
    }
    updateCaptureTab();
}

void ScoreboardOCR::closeEvent(QCloseEvent* event)
{
    if(!capManager->flags.testFlag(CaptureManager::captureStarted))
    {
        event->accept();
        return;
    }
    // Ask for app close confirmation if capture has been started
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

void ScoreboardOCR::displayMainImage(cv::Mat *img)
{
    mainGraphicsScene->clear();
    mainGraphicsScene->addPixmap(mat2pix(img));
    ui->mainImage->fitInView(mainGraphicsScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void ScoreboardOCR::displaySmallImage(cv::Mat *img)
{
    smallGraphicsScene->clear();
    smallGraphicsScene->addPixmap(mat2pix(img));
    ui->smallImage->fitInView(smallGraphicsScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

QPixmap ScoreboardOCR::mat2pix(cv::Mat *img)
{
    switch(img->type())
    {
    case CV_8UC4:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_ARGB32);
            return QPixmap::fromImage(qimg);
            break;
        }
    case CV_8UC3:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_RGB888);
            return QPixmap::fromImage(qimg.rgbSwapped());
            break;
        }
    case CV_8UC1:
        {
            QImage qimg(img->data, img->cols, img->rows, img->step, QImage::Format_Grayscale8);
            return QPixmap::fromImage(qimg);
            break;
        }
    default:
        qWarning() << "Mat2pix - unknown format!";
    }
    return QPixmap();
}

void ScoreboardOCR::doEdges()
{
    if(capManager->flags.testFlag(CaptureManager::edgesMarked))
    {
        capManager->startMarkingEdges();
    } else
    {
        capManager->clearEdges();
    }
    updateCaptureTab();
}

