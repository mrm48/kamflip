#include <QApplication>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

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
  QGroupBox *groupBox = new QGroupBox(tr("Options"));
  QCheckBox *flipInputVert = new QCheckBox("Flip Input Vertically");
  QCheckBox *setGreyscale = new QCheckBox("Greyscale");
  QComboBox *setWebcam = new QComboBox(this);
  QComboBox *setLoopback = new QComboBox(this);
  //QStringList *device = new QStringList("/dev/video0");
  QLabel *webcamLabel = new QLabel("Webcam:", this);
  QLabel *loopbackLabel = new QLabel("Loopback device:", this);
  setWebcam->insertItems(0, QStringList("/dev/video0"));
  setLoopback->insertItems(0, QStringList("/dev/video2"));
  webcamLabel->setBuddy(setWebcam);
  loopbackLabel->setBuddy(setLoopback);
  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->addWidget(webcamLabel);
  hbox->addWidget(setWebcam);
  QHBoxLayout *loopBox = new QHBoxLayout;
  loopBox->addWidget(loopbackLabel);
  loopBox->addWidget(setLoopback);
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(flipInputVert);
  vbox->addWidget(setGreyscale);
  vbox->addLayout(hbox);
  vbox->addLayout(loopBox);
  groupBox->setLayout(vbox);
  return groupBox;
}
