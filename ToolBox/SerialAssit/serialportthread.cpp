#include "serialportthread.h"
#include <QDebug>
#include <QByteArray>
#include <QString>
#include "serialassit.h"

SerialPortThread::SerialPortThread(QObject *parent):
    QObject(parent)
{
    this->pSerialPort = new QSerialPort();

    pThread = new QThread();
    this->moveToThread(pThread);
    connect(this->pSerialPort,&QSerialPort::readyRead,
            this,comread,Qt::QueuedConnection);
    //pThread->start();

//    connect(this, &SerialPortThread::serialDataReady,
//            parent, &SerialAssit::serialDataRev, Qt::QueuedConnection);

    if (pSerialPort == nullptr)
    {
        qDebug()<< "main pSerialPort is null!!!";
    }
    else
    {
        qDebug()<< "main pSerialPort is not null!!!";
    }
}

void SerialPortThread::restartThread(void)
{
   pThread->start();
   this->pSerialPort->setPortName(gComNum);
   if (this->pSerialPort->open(QIODevice::ReadWrite))
   {
       this->pSerialPort->setBaudRate(gComBaudRate);
       this->pSerialPort->setDataBits(QSerialPort::Data8);
       this->pSerialPort->setParity(QSerialPort::NoParity);
       this->pSerialPort->setStopBits(QSerialPort::OneStop);
       this->pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
       qDebug()<< "pSerialPort open success!!!";
   }
   else
   {
       qDebug()<< "pSerialPort open failed!!!";
   }
}
void SerialPortThread::exitThread(bool sw)
{
    pThread->quit();
    pThread->wait();
    pSerialPort->close();
}
void SerialPortThread::run()
{
    //pSerialPort->open()
    QByteArray arr;
    pSerialPort = new QSerialPort();
    if (pSerialPort->open(QIODevice::ReadWrite))
    {
        qDebug()<< "pSerialPort open success!!!";
    }
    else
    {
        qDebug()<< "pSerialPort open failed!!!";
        gThreadSwitch = false;
    }
    while(1)
    {
        //msleep(1000);
        if (gThreadSwitch == false)
            break;

        if (pSerialPort == nullptr)
        {
            qDebug()<< "pSerialPort is null!!!";
            continue;
        }
        QByteArray data ;
        data[0] = 't';
        data[1] = '0';
        pSerialPort->write(data);

        arr =  pSerialPort->readAll();
        if (!arr.isEmpty())
            qDebug()<< "thread is running!!!, len : "+QString::number(arr.length());

    }

    pSerialPort->close();
    gThreadSwitch = true;
}

void SerialPortThread::comread()
{
    QByteArray arr;
    arr = pSerialPort->readAll();

//    qDebug()<< "pSerialPort  receive data!!!" +
//               QString::number((int)QThread::currentThreadId())+
//               " len:"+ QString::number(arr.length()); ;
    emit serialDataReady(arr);
}
void SerialPortThread::setComNum(QString str)
{
    gComNum.clear();
    gComNum = str;
}
void SerialPortThread::setBaudRate(qint32 baudrate)
{

    gComBaudRate = baudrate;
}
