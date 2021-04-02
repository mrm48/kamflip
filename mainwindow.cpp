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
  QCheckBox *flipInputVert = new QCheckBox("Flip Input Vertically");
  QCheckBox *setGreyscale = new QCheckBox("Greyscale");
  
  // Set up Dropdowns
  QComboBox *setWebcam = new QComboBox(this);
  QComboBox *setLoopback = new QComboBox(this);
  QStringList device = *(new QStringList("/dev/video0")); // TODO: populate this list with potential devices
  QStringList loopback = *(new QStringList("/dev/video2")); // TODO: populate this list with potential devices
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
  QProcess getDeviceProcess;
  getDeviceProcess.start("ls", QStringList() << "/dev/");
  getDeviceProcess.waitForReadyRead(); 
  QByteArray devices = getDeviceProcess.readAll();
  QString devicesString = devices.data();
  QStringList deviceList = *(new QStringList(devicesString.split("\n")));
  QStringList trimmedDevList = *(new QStringList());
  QString tempDev = "";
  QStringListIterator trimIterator(deviceList);
  while (trimIterator.hasNext()){
    tempDev = trimIterator.next();
    if(tempDev.contains("video"))
      trimmedDevList.append(tempDev);
  }
  return trimmedDevList; 
}