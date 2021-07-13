#include <QApplication>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>

#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>

#include "mainwindow.h"

QCheckBox *flipInputVert;

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
  setCentralWidget(createOptionBox());
}

void MainWindow::setupActions()
{
    KStandardAction::quit(qApp, &QCoreApplication::quit, actionCollection());
    setupGUI(Default, "kamflipui.rc");
}

QGroupBox* MainWindow::createOptionBox()
{

  // Instantiate group
  QGroupBox *groupBox = new QGroupBox(tr("Options"));

  // Instantiate checkboxes for various filters / options
  //QCheckBox *flipInputVert = new QCheckBox("Flip Input Vertically");
  flipInputVert = new QCheckBox("Flip Input Vertically");
  connect(flipInputVert,SIGNAL(clicked(bool)), this, SLOT(updateFlip(bool)));
  QCheckBox *setGreyscale = new QCheckBox("Greyscale");
  
  // Set up Dropdowns
  QComboBox *setWebcam = new QComboBox(this);
  QComboBox *setLoopback = new QComboBox(this);
  QLabel *webcamLabel = new QLabel("Webcam:", this);
  QLabel *loopbackLabel = new QLabel("Loopback device:", this);
  setWebcam->insertItems(0, getDevices());
  setLoopback->insertItems(0, getDevices());
  webcamLabel->setBuddy(setWebcam);
  loopbackLabel->setBuddy(setLoopback);
  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->addWidget(webcamLabel);
  hbox->addWidget(setWebcam);
  QHBoxLayout *loopBox = new QHBoxLayout;
  loopBox->addWidget(loopbackLabel);
  loopBox->addWidget(setLoopback);

  // Set up vertical layout
  // 1. Checkboxes 2. labels and dropdowns
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(flipInputVert);
  vbox->addWidget(setGreyscale);
  vbox->addLayout(hbox);
  vbox->addLayout(loopBox);

  // return layout - vbox is wrapper around 1. Checkboxes and 2. horizontal layouts containing labels and dropdowns
  groupBox->setLayout(vbox);
  return groupBox;
}

QStringList MainWindow::getDevices()
{
  // Get list of all devices by pulling listing from /dev
  QProcess getDeviceProcess;
  getDeviceProcess.start("ls", QStringList() << "/dev/");
  getDeviceProcess.waitForReadyRead(); 
  QByteArray devices = getDeviceProcess.readAll();

  // Parse output of ls and add devices into an iterator 
  QString devicesString = devices.data();
  QStringList deviceList = *(new QStringList(devicesString.split("\n")));
  QStringList trimmedDevList = *(new QStringList());
  QString tempDev = "";
  QStringListIterator trimIterator(deviceList);
  
  // Filter down to only webcams and loopback devices for video
  while (trimIterator.hasNext()){
    tempDev = trimIterator.next();
    if(tempDev.contains("video"))
      trimmedDevList.append(tempDev);
  }

  return trimmedDevList; 
}

// Action when flip checkbox is selected
void MainWindow::updateFlip(bool value)
{

  if(value)
  {
    QMessageBox msgBox;
    msgBox.setText("Flip is checked");
    msgBox.exec();
  } else
  {
    QMessageBox msgBox;
    msgBox.setText("Flip is unchecked");
    msgBox.exec();
  }
}
