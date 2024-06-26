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

    outManager->addSelections(recManager->getSelections());

    // Initialize UI
    ui->setupUi(this);
    addFormatsToOutputTab();
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

    // Connect UI signals/slots
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));

    connect(ui->edgesButton, SIGNAL(released()), this, SLOT(doEdges()));                                        // Edges button pressed
    connect(ui->captureButton, SIGNAL(released()), this, SLOT(doCapture()));                                    // Capture button pressed
    connect(ui->refreshCaptureDeviceButton, SIGNAL(released()), this, SLOT(updateDeviceDropdown()));            // Refresh capture device dropdown
    connect(ui->captureDeviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDevice(int)));    // Combobox device select
    connect(ui->captureTypeTab, SIGNAL(currentChanged(int)), this, SLOT(setCaptureType(int)));                  // Capture type tab selected
    connect(ui->ipAddressBox, SIGNAL(textChanged(QString)), this, SLOT(setCameraIPAddress(QString)));           // Set IP address of camera

    connect(ui->blurSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBlur(int)));                // Update blur
    connect(ui->gapsSlider, SIGNAL(valueChanged(int)), this, SLOT(changeGaps(int)));                // Update gaps
    connect(ui->erodeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeErode(int)));              // Update erode
    connect(ui->italicSlider, SIGNAL(valueChanged(int)), this, SLOT(changeItalic(int)));            // Update italic
    connect(ui->dialateSlider, SIGNAL(valueChanged(int)), this, SLOT(changeDialate(int)));          // Update dialate
    connect(ui->invertColorsCheckbox, SIGNAL(toggled(bool)), this, SLOT(changeInvert(bool)));       // Update color invert
    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(changeThreshold(int)));      // Update threshold

    connect(ui->addSelectionButton, SIGNAL(released()), this, SLOT(addSelection()));                // Add selection button pressed
    connect(ui->deleteSelectionButton, SIGNAL(released()), this, SLOT(deleteSelection()));          // Delete selection button pressed

    connect(ui->outputPathButton, SIGNAL(released()), this, SLOT(setOutputPath()));                         // Ask user for path where to save outout
    connect(ui->outputNameLineEdit, SIGNAL(editingFinished()), this, SLOT(setOutputFilename()));            // Set output file name
    connect(ui->outputFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setOutputFormat(int)));  // Set output format

    // Load settings
    settingsManager = new SettingsManager();
    settingsManager->addMainWorker(mainWorker);
    settingsManager->addOutputManager(outManager);
    settingsManager->addRecognitionManager(recManager);
    settingsManager->loadSettings();

    // Initialize SVM
    recManager->loadSVM();

    // Update device combobox
    updateDeviceDropdown();

    // Update UI with values
    updateOutputTab();
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

void ScoreboardOCR::updateDeviceDropdown()
{
    ui->captureDeviceComboBox->clear();
    for (Camera &cam : capManager->getDevices())
    {
        #ifndef Q_OS_UNIX
        // Windows camera id is "ugly" so we dont show info
        ui->captureDeviceComboBox->addItem(cam.description);
        #else
        ui->captureDeviceComboBox->addItem(cam.name + " | " + cam.description);
        #endif
    }
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
        ui->refreshCaptureDeviceButton->setEnabled(false);
        ui->captureTypeTab->setEnabled(false);
    } else
    {
        ui->captureButton->setText("Start capture");
        ui->edgesButton->setEnabled(false);
        ui->refreshCaptureDeviceButton->setEnabled(true);
        ui->captureTypeTab->setEnabled(true);
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
    if (recManager->flags.testFlag(RecognitionManager::selectingSelection) || !capManager->flags.testFlag(CaptureManager::edgesMarked))
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

void ScoreboardOCR::setCaptureType(int val)
{
    CaptureManager::CaptureType type;
    switch (val) {
    case 0:
        type = CaptureManager::USB;
        break;
    case 1:
        type = CaptureManager::IP;
        break;
    default:
        type = CaptureManager::USB;
    }
    capManager->changeCaptureType(type);
    updateCaptureTab();
}

void ScoreboardOCR::setCameraIPAddress(QString str)
{
    capManager->setCameraIPAddress(str);
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
            QMessageBox::critical(this, tr("Error"), tr("Cannot open capture device! Make sure it is working and plugged in, or IP address is correct!"), QMessageBox::Close);
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
    updateRecognitionTab();
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
    // Resize image if it is too big
    if(ui->mainImage->width() - ui->mainImage->viewport()->width() > 10)
      ui->mainImage->fitInView(mainGraphicsScene->getMainPixmap(), Qt::KeepAspectRatio);

    mainGraphicsScene->paintBackground(img);
}

void ScoreboardOCR::displaySmallImage(cv::Mat *img)
{
    // Resize image if it is too big
    if(ui->smallImage->width() - ui->smallImage->viewport()->width() > 10)
      ui->smallImage->fitInView(smallGraphicsScene->getMainPixmap(), Qt::KeepAspectRatio);
    smallGraphicsScene->paintBackground(img);
}

void ScoreboardOCR::doEdges()
{
    if(capManager->flags.testFlag(CaptureManager::edgesMarked))
    {
        capManager->clearEdges();
        mainGraphicsScene->clearEdgePoints();
        mainGraphicsScene->stopShowingSelections();
    } else
    {
        capManager->startMarkingEdges();
    }
    updateCaptureTab();
    updateRecognitionTab();
}

void ScoreboardOCR::updateEdges(QList<QPoint> points)
{
    capManager->setEdges(points);
    mainGraphicsScene->startShowingSelections();
    updateCaptureTab();
    updateRecognitionTab();
}

void ScoreboardOCR::changeInvert(bool val)
{
    filManager->setInvert(val);
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

    // Add selection names to dialog so that we can add only unique names to selections
    QList<QString> uniqueNames;
    for(Selection &selection : *recManager->getSelections())
    {
        uniqueNames.append(selection.getName());
    }
    dialog.setUniqueNames(uniqueNames);

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
    int i = ui->recognitionListWidget->currentRow();        // Get index of current selection
    ui->recognitionListWidget->removeItemWidget(selection);
    ui->recognitionListWidget->takeItem(i);
    mainGraphicsScene->removeSelection(i);
    recManager->deleteSelection(i);
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
    settingsManager->saveSettings(nullptr);
}

void ScoreboardOCR::setOutputFilename()
{
    QString name = ui->outputNameLineEdit->text();
    if(name.isEmpty())
        return;
    outManager->setOutputFilename(name);
    settingsManager->saveSettings(nullptr);
}

void ScoreboardOCR::setOutputFormat(int i)
{
    outManager->setOutputFormat(static_cast<OutputManager::OutputFormat>(i));
    settingsManager->saveSettings(nullptr);
}

void ScoreboardOCR::addFormatsToOutputTab()
{
    if(ui->outputFormatComboBox->count() == 0)
    {
        ui->outputFormatComboBox->addItems(outManager->getFormatDescriptions());
    }
}

void ScoreboardOCR::updateOutputTab()
{
    ui->outputNameLineEdit->setText(outManager->getOutputFilename());
    ui->outputPathLineEdit->setText(outManager->getOutputPath());
    ui->outputFormatComboBox->setCurrentIndex(outManager->getOutputFormat());
}
