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

        gRevDataLen = 0;
        gRevBuffer.clear();
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

QString SerialPortThread::ByteArrayToString(QByteArray &ba)
{
    QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
    QString buf;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每次一个字节的填充到 outchar
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
        buf += str;
    }
    return buf;
}

void SerialPortThread::comread()
{
    QByteArray arr;
    arr = pSerialPort->readAll();

//        qDebug()<< "thread04:";
//        qDebug()<<QThread::currentThreadId();

    quint16 length = arr.length();
    if (length %2)
    {
        arr.remove(length, 1);
        length -= 1;

        qDebug()<< "thread04:";
        qDebug()<<QThread::currentThreadId();
    }

    //mux = 0;
    gRevBuffer.append(arr);
    gRevDataLen = gRevDataLen + length;
    //mux = 1;

//    {
//        QString fileout = "./AudioFile0.pcm";
//        QFile *inputFile = new QFile(fileout);

//        inputFile->setFileName(fileout);
//        if (false == inputFile->open(QIODevice::WriteOnly))
//        {
//            return ;
//        }

//        inputFile->write(gRevBuffer);
//        inputFile->close();
//    }

    emit serialDataReady();
}
void SerialPortThread::comwrite(QByteArray arr)
{
    if (pSerialPort->isOpen())
        this->pSerialPort->write(arr);
    else
        qDebug()<< "[SerialPortThread]pSerialPort is not opened!!!";

    //        qDebug()<< "thread03:";
    //        qDebug()<<QThread::currentThreadId();
}

quint32 SerialPortThread::getCurrentRevLength(void)
{
    //if (mux == 0)
    // return 0;
    return gRevDataLen;
}
QByteArray SerialPortThread::getRevDataArr(int start, int end)
{
    //if (mux == 0)
    {
        //return 0;
    }
    if (start > end)
        return gRevBuffer.left(0);

    return gRevBuffer.mid(start, end-start);
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
