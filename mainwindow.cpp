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
QCheckBox *setGreyscale;
QComboBox *setWebcam;
QComboBox *setLoopback;

bool debug = false;

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
  setGreyscale = new QCheckBox("Greyscale");
  connect(setGreyscale,SIGNAL(clicked(bool)), this, SLOT(updateGreyscale(bool)));
  
  // Set up Dropdowns
  setWebcam = new QComboBox(this);
  setLoopback = new QComboBox(this);
  QLabel *webcamLabel = new QLabel("Webcam:", this);
  QLabel *loopbackLabel = new QLabel("Loopback device:", this);
  setWebcam->insertItems(0, getDevices());
  setLoopback->insertItems(0, getLoopback());
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
  getDeviceProcess.start("ls", { "/dev/" });
  getDeviceProcess.waitForReadyRead(); 
  QByteArray devices = getDeviceProcess.readAll();
  getDeviceProcess.close();
  // Parse output of ls and add devices into an iterator 
  QString devicesString = devices.data();
  QStringList deviceList = *(new QStringList(devicesString.split("\n")));
  QStringList trimmedDevList = *(new QStringList());
  QString tempDev = "";
  QStringListIterator trimIterator(deviceList);
  
  // Filter down to only webcams and loopback devices for video
  while (trimIterator.hasNext()){
    tempDev = trimIterator.next();
    if(tempDev.contains("video")){
      trimmedDevList.append(tempDev);
    }
  }

  return trimmedDevList;
}

QStringList MainWindow::getLoopback()
{
  // Get list of all devices by pulling listing from /dev
  QProcess getDeviceProcess;
  getDeviceProcess.start("sh", {"-c", "v4l2-ctl --list-devices | sed -n '/v4l2loopback/,+1p'"});
  QByteArray devices;
  if(getDeviceProcess.waitForReadyRead()){
    devices = getDeviceProcess.readAll();
    getDeviceProcess.close();
  }
  // Parse output of ls and add devices into an iterator 
  QString devicesString = devices.data();
  QStringList deviceList = *(new QStringList(devicesString.split("\n")));
  QStringList trimmedDevList = *(new QStringList());
  QString tempDev = "";
  QStringListIterator trimIterator(deviceList);
  
  // Filter down to only webcams and loopback devices for video
  while (trimIterator.hasNext()){
    tempDev = trimIterator.next();
    if(tempDev.contains("/dev/")){
      tempDev = tempDev.remove("/dev/");
      trimmedDevList.append(tempDev);
    }
  }
  if(trimmedDevList.length() == 0){
    trimmedDevList.append("Error: no loopback found");
    flipInputVert->setEnabled(false);
    setGreyscale->setEnabled(false);
  }
  return trimmedDevList;
}

// Action when flip checkbox is toggled
// TODO: Add call to v4l2 that will flip the camera
void MainWindow::updateFlip(bool value)
{

  if(value)
  {

    QProcess flipProcess;
    flipProcess.start("sh", {"-c", "ffmpeg -f v4l2 -i", setWebcam->currentText(), "-vf vflip -f v4l2 ", setLoopback->currentText()});
    if(flipProcess.waitForStarted()){
      flipProcess.waitForReadyRead();
    }
    setGreyscale->setEnabled(false);
    
    if(debug){

      QMessageBox msgBox;
      msgBox.setText("Flip is checked");    
      msgBox.exec();

    }

  }

  else
  {

    setGreyscale->setEnabled(true);

    if(debug)
    {

      QMessageBox msgBox;
      msgBox.setText("Flip is unchecked"); 
      msgBox.exec();

    }

  }
}

// Action when greyscale checkbox is toggled
// TODO: Add call to v4l2 to change output of webcam to black & white
void MainWindow::updateGreyscale(bool value)
{

  if(value)
  {

    QProcess flipProcess;
    flipProcess.start("sh", {"-c", "ffmpeg -f v4l2 -i ", setWebcam->currentText(), "-vf eq=gamma=1.5:saturation=0 -f v4l2 ", setLoopback->currentText()});
    flipInputVert->setEnabled(false);
    
    if(debug)
    {

      QMessageBox msgBox;
      msgBox.setText("Greyscale is checked");
      msgBox.exec();

    }
    
  }

  else
  {

    flipInputVert->setEnabled(true);

    if(debug)
    {

      QMessageBox msgBox;
      msgBox.setText("Greyscale is unchecked");
      msgBox.exec();

    }

  }

}

// Terminate ffmpeg and restart with filters that are selected
void MainWindow::reapplyFilters(){

  QProcess reapplyProcess;
  reapplyProcess.start("sh", {"-c", "killall ffmpeg"});
  reapplyProcess.close();
    
}
