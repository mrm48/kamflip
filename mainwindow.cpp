#include <QApplication>
#include <QAction>
#include <QSaveFile>
#include <QFileDialog>
#include <QTextStream>
#include <QByteArray>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>

#include <KTextEdit>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
#include <KIO/Job>

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
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(flipInputVert);
  vbox->addWidget(setGreyscale);
  groupBox->setLayout(vbox);
  return groupBox;
}
