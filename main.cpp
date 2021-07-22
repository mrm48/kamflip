#include <cstdlib>
#include <QApplication>
#include <QCommandLineParser>
#include <KAboutData>
#include <KLocalizedString>
#include "mainwindow.h"
 
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kamflip");
    
    KAboutData aboutData(
                         QStringLiteral("kamflip"),
                         i18n("Kamflip"),
                         QStringLiteral("1.0"),
                         i18n("A simple application for controlling v4l2loopback devices"),
                         KAboutLicense::GPL,
                         i18n("(c) 2021"),
                         i18n("Requires v4l2loopback"),
                         QStringLiteral("https://github.com/mrm48/kamflip"),
                         QStringLiteral("submit@bugs.kde.org"));

    KAboutData::setApplicationData(aboutData);
 
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    
    MainWindow *window = new MainWindow();
    window->show();
    
    return app.exec();
}
