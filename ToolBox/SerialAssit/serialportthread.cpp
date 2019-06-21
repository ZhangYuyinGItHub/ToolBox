#include "serialportthread.h"
#include <QDebug>
#include <QByteArray>
#include <QString>
#include "serialassit.h"

SerialPortThread::SerialPortThread(QObject *parent):
    QObject(parent)
{
    pThread = new QThread();
    this->moveToThread(pThread);

    /*
     * note: 所有使用pThread触发的槽函数都是运行在该线程中；
     *       serialThreadStarted 运行在thread中，里面运行的函数触发的槽函数(comread())也是运行在该线程中；
     */
    connect(pThread, &QThread::started, this, &SerialPortThread::serialThreadStarted,
            Qt::QueuedConnection);

    pThread->start();

}

void SerialPortThread::serialThreadStarted()
{
    if (pSerialPort == nullptr)
    {
        qDebug()<< "main pSerialPort is null!!!";
        this->pSerialPort = new QSerialPort();
    }
    else
    {
        qDebug()<< "main pSerialPort is not null!!!";
    }

    //pSerialPort->moveToThread(pThread);
    connect(this->pSerialPort,&QSerialPort::readyRead,
            this, comread, Qt::QueuedConnection);

    qDebug()<< "thread01:";
    qDebug()<<QThread::currentThreadId();
}
void SerialPortThread::restartThread(void)
{
    if (!pThread->isRunning())
    {
        qDebug()<< "restart serial thread\r\n";
        pThread->start();
    }

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
    //pThread->quit();
    //pThread->terminate();
    pSerialPort->clear();
    pSerialPort->close();
    qDebug()<< "pSerialPort thread exit!!!";
}


void SerialPortThread::comread()
{
    QByteArray arr;
    arr = pSerialPort->readAll();

    emit serialDataReady(arr);
}
void SerialPortThread::comwrite(QByteArray arr)
{
    if (pSerialPort->isOpen())
        this->pSerialPort->write(arr);
    else
        qDebug()<< "[SerialPortThread]pSerialPort is not opened!!!";

    //    qDebug()<< "thread03:";
    //    qDebug()<<QThread::currentThreadId();
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
