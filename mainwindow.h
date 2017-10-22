#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QList>
#include <QIODevice>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void StartSlot();
    void ResetSlot();
    void SelectPortSlot();
    void SelectBaudRateSlot();

    void HandlingReadDataSlot();
    void WriteDataSlot();

private:
    Ui::MainWindow *ui;
    QSerialPort *mSerialPort;
    QTimer* mTimer;
    void InitUI();
};

#endif // MAINWINDOW_H
