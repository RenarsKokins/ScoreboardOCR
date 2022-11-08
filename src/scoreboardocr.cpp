#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidgetItem>
#include "scoreboardocr.h"
#include "selectionwidget.h"
#include "selectiondialog.h"
#include "ui_scoreboardocr.h"

ScoreboardOCR::ScoreboardOCR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreboardOCR)
{
    // Initialize managers
    filManager = new FilterManager();
    outManager = new OutputManager();
    capManager = new CaptureManager();
    recManager = new RecognitionManager();

    // Initialize UI
    ui->setupUi(this);
    smallGraphicsScene = new CaptureScene(this);
    mainGraphicsScene = new MainCaptureScene(this, capManager, recManager);
    ui->mainImage->setScene(mainGraphicsScene);
    ui->smallImage->setScene(smallGraphicsScene);
    connect(mainGraphicsScene, SIGNAL(updateEdges(QList<QPoint>)), this, SLOT(updateEdges(QList<QPoint>)));
    connect(mainGraphicsScene, SIGNAL(updateSelectionCoordinates(QRect)), this, SLOT(updateSelectionCoordinates(QRect)));

    // Initialize main worker thread
    mainWorker = new MainWorker();
    mainWorker->moveToThread(&workerThread);
    mainWorker->addFilterManager(filManager);
    mainWorker->addOutputManager(outManager);
    mainWorker->addCaptureManager(capManager);
    mainWorker->addRecognitionManager(recManager);
    connect(this, SIGNAL(startMainWorker()), mainWorker, SLOT(doWork()));
    connect(mainWorker, SIGNAL(setMainImage(cv::Mat*)), this, SLOT(displayMainImage(cv::Mat*)));
    connect(mainWorker, SIGNAL(setSmallImage(cv::Mat*)), this, SLOT(displaySmallImage(cv::Mat*)));

    // Update device combobox
    updateDeviceDropdown();

    // Connect UI signals/slots
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));

    connect(ui->edgesButton, SIGNAL(released()), this, SLOT(doEdges()));                                        // Edges button pressed
    connect(ui->captureButton, SIGNAL(released()), this, SLOT(doCapture()));                                    // Capture button pressed
    connect(ui->captureDeviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDevice(int)));    // Combobox device select

    connect(ui->blurSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBlur(int)));                // Update blur
    connect(ui->gapsSlider, SIGNAL(valueChanged(int)), this, SLOT(changeGaps(int)));                // Update gaps
    connect(ui->erodeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeErode(int)));              // Update erode
    connect(ui->italicSlider, SIGNAL(valueChanged(int)), this, SLOT(changeItalic(int)));            // Update italic
    connect(ui->dialateSlider, SIGNAL(valueChanged(int)), this, SLOT(changeDialate(int)));          // Update dialate
    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(changeThreshold(int)));      // Update threshold

    connect(ui->addSelectionButton, SIGNAL(released()), this, SLOT(addSelection()));                // Add selection button pressed
    connect(ui->deleteSelectionButton, SIGNAL(released()), this, SLOT(deleteSelection()));          // Delete selection button pressed

    connect(ui->outputPathButton, SIGNAL(released()), this, SLOT(setOutputPath()));                 // Ask user for path where to save outout
    connect(ui->outputNameLineEdit, SIGNAL(editingFinished()), this, SLOT(setOutputFilename()));    // Set output file name
    // Load settings
    settingsManager = new SettingsManager();
    settingsManager->addMainWorker(mainWorker);
    settingsManager->addRecognitionManager(recManager);
    settingsManager->loadSettings();

    // Initialize SVM
    recManager->loadSVM();
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
    delete filManager;
    delete recManager;
    delete mainWorker;
    delete settingsManager;
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
        ui->edgesButton->setDisabled(false);
        ui->edgesButton->setText("Clear edges");
    } else if (capManager->flags.testFlag(CaptureManager::capturingEdges))
    {
        ui->edgesButton->setDisabled(true);
        ui->edgesButton->setText("Marking edges...");
    } else
    {
        ui->edgesButton->setDisabled(false);
        ui->edgesButton->setText("Mark edges");
    }
}

void ScoreboardOCR::updateRecognitionTab()
{
    if (recManager->flags.testFlag(RecognitionManager::selectingSelection))
    {
        ui->addSelectionButton->setEnabled(false);
        ui->deleteSelectionButton->setEnabled(false);
    } else
    {
        ui->addSelectionButton->setEnabled(true);
        ui->deleteSelectionButton->setEnabled(true);
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
        if(workerThread.isRunning())
        {
            workerThread.requestInterruption();
            workerThread.quit();
            workerThread.wait();
        }
        if(!capManager->stopCapture())
            QMessageBox::warning(this, tr("Warning"), tr("No capture device has been opened so capture has already been stopped!"), QMessageBox::Close);
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

void ScoreboardOCR::resizeEvent(QResizeEvent *event)
{
    ui->mainImage->fitInView(mainGraphicsScene->getMainPixmap(), Qt::KeepAspectRatio);
    ui->smallImage->fitInView(smallGraphicsScene->getMainPixmap(), Qt::KeepAspectRatio);
}

void ScoreboardOCR::displayMainImage(cv::Mat *img)
{
    mainGraphicsScene->paintBackground(img);
    // mainGraphicsScene->paintForeground();
}

void ScoreboardOCR::displaySmallImage(cv::Mat *img)
{
    smallGraphicsScene->paintBackground(img);
}

void ScoreboardOCR::doEdges()
{
    if(capManager->flags.testFlag(CaptureManager::edgesMarked))
    {
        capManager->clearEdges();
        mainGraphicsScene->clearEdgePoints();
    } else
    {
        capManager->startMarkingEdges();
    }
    updateCaptureTab();
}

void ScoreboardOCR::updateEdges(QList<QPoint> points)
{
    capManager->setEdges(points);
    updateCaptureTab();
}

void ScoreboardOCR::changeGaps(int val)
{
    filManager->setFillGaps(val);
}

void ScoreboardOCR::changeBlur(int val)
{
    filManager->setBlur(val);
}

void ScoreboardOCR::changeErode(int val)
{
    filManager->setErode(val);
}

void ScoreboardOCR::changeItalic(int val)
{
    filManager->setItalic(val);
}

void ScoreboardOCR::changeDialate(int val)
{
    filManager->setDialate(val);
}

void ScoreboardOCR::changeThreshold(int val)
{
    filManager->setThreshold(val);
}

void ScoreboardOCR::addSelection()
{
    SelectionDialog dialog(this);
    bool accepted = 0;
    accepted = dialog.exec();
    if(!accepted)
        return;
    Selection *sel = recManager->addSelection(dialog.getName(), dialog.getType());  // Add selection in recognition manager
    SelectionWidget *selWid = new SelectionWidget();    // Create widget for UI list
    selWid->addSelection(sel);                          // Add selection data to widget
    QListWidgetItem *item = new QListWidgetItem();      // Create widget item to put widget in
    item->setSizeHint(selWid->sizeHint());
    ui->recognitionListWidget->addItem(item);
    ui->recognitionListWidget->setItemWidget(item, selWid);
    selWid->updateSelection();
    updateRecognitionTab();
}

void ScoreboardOCR::deleteSelection()
{
    QListWidgetItem *selection = ui->recognitionListWidget->currentItem();
    if(!selection)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Choose a selection to be deleted!"), QMessageBox::Ok);
        return;
    }
    int i = ui->recognitionListWidget->currentRow();    // Save index of current selection
    SelectionWidget *selWid = static_cast<SelectionWidget *>(ui->recognitionListWidget->itemWidget(selection));    // Get SelectionWidget pointer
    recManager->deleteSelection(selWid->getSelection());    // Remove selection from RecognitionManager
    ui->recognitionListWidget->removeItemWidget(selection); // Remove selection from widget list
    mainGraphicsScene->removeSelection(i);                  // Remove selection from graphics scene at index
    delete ui->recognitionListWidget->itemWidget(selection);
    delete selection;
    updateRecognitionTab();
}

void ScoreboardOCR::updateSelectionCoordinates(QRect rect)
{
    recManager->updateSelectionCoordinates(rect);
    updateRecognitionTab();
}

void ScoreboardOCR::openSettings()
{
    settingsManager->showSettingsDialog(this);
}

void ScoreboardOCR::setOutputPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Path where to save output"), ".", QFileDialog::ShowDirsOnly);
    if(path.isEmpty())
        return;
    outManager->setOutputPath(path);
    ui->outputPathLineEdit->setText(outManager->getOutputPath());
}

void ScoreboardOCR::setOutputFilename()
{
    QString name = ui->outputNameLineEdit->text();
    if(name.isEmpty())
        return;
    outManager->setOutputFilename(name);
}
