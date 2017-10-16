#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //PushButoon Slot
    QObject::connect(ui->pButton_Start, SIGNAL(clicked(bool)), this, SLOT(StartSlot()));
    QObject::connect(ui->pButton_Reset, SIGNAL(clicked(bool)), this, SLOT(ResetSlot()));
   // QObject::connect(ui->comboBox_port,SIGNAL(), this, SLOT());
    //Init Serial Port
    InitSerialPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartSlot()
{
    mSerialPort = new QSerialPort;
    QSerialPort::BaudRate mBaudRate;

    switch(ui->comboBox_baudrate->currentIndex())
    {
        case 0: mBaudRate = QSerialPort::Baud115200; break;
        case 1: mBaudRate = QSerialPort::Baud57600; break;
        case 2: mBaudRate = QSerialPort::Baud38400; break;
        case 3: mBaudRate = QSerialPort::Baud19200; break;
        case 4: mBaudRate = QSerialPort::Baud9600; break;
        default: mBaudRate = QSerialPort::Baud57600; break;
    }

    mSerialPort->setBaudRate(mBaudRate);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    bool portStatus = mSerialPort->open(QIODevice::ReadWrite);
    if(portStatus){
        ui->label_PortStatus->setText("已开启");
    }else{
        ui->label_PortStatus->setText("打开失败");
    }
}

void MainWindow::ResetSlot()
{
    ui->label_PortStatus->setText("已关闭");
    ui->lineEdit_Cnt->setText("00");
}

void MainWindow::SelectPortSlot()
{

}

void MainWindow::SelectBaudRateSlot()
{

}

void MainWindow::InitSerialPort()
{
    QList<QSerialPortInfo> portList;
    QSerialPortInfo *mSerialPortInfo = new QSerialPortInfo;

    portList = mSerialPortInfo->availablePorts();
    QMessageBox::information(this,"info",QString::number(portList.size()));
    for(int i=0; i < portList.size(); i++)
    {
        ui->comboBox_port->addItem(portList.at(i).portName());
    }
}
