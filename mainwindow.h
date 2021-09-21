#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <KXmlGuiWindow>
#include <QGroupBox>
#include <QMessageBox>
 
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void updateFlip(bool);
    void updateGreyscale(bool);

private:
    void setupActions();
    QStringList getDevices();
    QStringList getLoopback();
    QGroupBox *createOptionBox();

};

#endif // MAINWINDOW_H
