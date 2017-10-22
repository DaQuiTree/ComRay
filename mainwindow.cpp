#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSerialPort = new QSerialPort;

    //PushButoon Slot
    QObject::connect(ui->pButton_Start, SIGNAL(clicked(bool)), this, SLOT(StartSlot()));
    QObject::connect(ui->pButton_Reset, SIGNAL(clicked(bool)), this, SLOT(ResetSlot()));
    QObject::connect(this->mSerialPort, SIGNAL(readyRead()), this, SLOT(HandlingReadDataSlot()));

    //Init Timer
    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(WriteDataSlot()));

    //Init UI
    InitUI();
}

MainWindow::~MainWindow()
{
    delete mSerialPort;
    delete ui;
}

void MainWindow::StartSlot()
{
    QSerialPort::BaudRate mBaudRate;
    QSerialPort::Parity mParity;
    static bool portStatus = 0;

    if(!portStatus)
    {
        mSerialPort->setPortName(ui->comboBox_port->currentText());//port

        switch(ui->comboBox_baudrate->currentIndex())//baudRate
        {
            case 0: mBaudRate = QSerialPort::Baud115200; break;
            case 1: mBaudRate = QSerialPort::Baud57600; break;
            case 2: mBaudRate = QSerialPort::Baud38400; break;
            case 3: mBaudRate = QSerialPort::Baud19200; break;
            case 4: mBaudRate = QSerialPort::Baud9600; break;
            default: mBaudRate = QSerialPort::Baud57600; break;
        }
        mSerialPort->setBaudRate(mBaudRate);

        switch(ui->comboBox_Parity->currentIndex())//baudRate
        {
            case 0: mParity = QSerialPort::NoParity; break;
            case 1: mParity = QSerialPort::EvenParity; break;
            case 2: mParity = QSerialPort::OddParity; break;
            default: mParity = QSerialPort::NoParity; break;
        }
        mSerialPort->setParity(mParity);

        mSerialPort->setDataBits(QSerialPort::Data8); //other
        mSerialPort->setStopBits(QSerialPort::OneStop);
        mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

        portStatus = mSerialPort->open(QIODevice::ReadWrite);
        if(portStatus){
            ui->label_PortStatus->setText("串口已开启");
            ui->pButton_Start->setText("close");
            mTimer->start(200); //启动发送
        }else{
            ui->label_PortStatus->setText("打开失败");
        }
    }else{
        mTimer->stop();
        mSerialPort->close();
        portStatus = 0;
        ui->label_PortStatus->setText("串口已关闭");
        ui->pButton_Start->setText("start");
    }
}

void MainWindow::ResetSlot()
{
    ui->lineEdit_Cnt->setText("00 00");
}

void MainWindow::SelectPortSlot()
{

}

void MainWindow::SelectBaudRateSlot()
{

}

void MainWindow::InitUI()
{
    //Init combobox baudrate & parity
    ui->comboBox_baudrate->setCurrentText("57600");//57600
    ui->comboBox_Parity->setCurrentText("None");//parityNone

    //Init Serial Port
    QList<QSerialPortInfo> portList;
    QSerialPortInfo *mSerialPortInfo = new QSerialPortInfo;

    portList = mSerialPortInfo->availablePorts();
    for(int i=0; i < portList.size(); i++)
    {
        ui->comboBox_port->addItem(portList.at(i).portName());
    }
    delete mSerialPortInfo;
}

void MainWindow::HandlingReadDataSlot()
{
    int numRead = 0;
    char buffer[64] = {0};
    char code[] = "00 00";
    char hexMap[] ={'0','1','2','3','4','5','6','7',
                    '8','9','A','B','C','D','E','F'};

    numRead  = mSerialPort->read(buffer, 64);
    if(numRead == 0)return;

    code[0] = hexMap[(unsigned char)buffer[4]/16];
    code[1] = hexMap[(unsigned char)buffer[4]%16];
    code[3] = hexMap[(unsigned char)buffer[5]/16];
    code[4] = hexMap[(unsigned char)buffer[5]%16];

    QString str(code);
    //QMessageBox::information(this,"info",QString::fromUtf8(code));
    ui->lineEdit_Cnt->setText(str);

}

void MainWindow::WriteDataSlot()
{
    static int pos = 1;
    char writeBuf[2] = {0x01, 0x55};

    writeBuf[0] = pos++;
    if(pos > 4)pos = 1;
    mSerialPort->write(writeBuf, 2);
}
