#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <KXmlGuiWindow>
#include <QGroupBox>

class KTextEdit;
class KJob;
 
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupActions();
    QStringList getDevices();

private:
    QGroupBox *createOptionBox();

};

#endif // MAINWINDOW_H
