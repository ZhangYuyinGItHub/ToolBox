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

    myCom = nullptr;
    pThread->start();

}

void SerialPortThread::serialThreadStarted()
{
    if (myCom == nullptr)
    {
        qDebug()<< "main pSerialPort is null!!!";
        //定义一个结构体，用来存放串口各个参数
        struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
        //定义串口对象，并传递参数，在构造函数里对其进行初始化
        myCom = new Win_QextSerialPort("com3",myComSetting,QextSerialBase::EventDriven);

    }
    else
    {
        qDebug()<< "main pSerialPort is not null!!!";
    }

    connect(myCom, &Win_QextSerialPort::readyRead, this, comread, Qt::QueuedConnection);

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

    ComBaud mBaud;
    if (gComBaudRate == tr("9600"))
    {
        mBaud = BAUD9600;
    }
    else if (gComBaudRate == tr("115200"))
    {
        mBaud = BAUD115200;
    }
    else if (gComBaudRate == tr("2000000"))
    {
        mBaud = BAUD2000000;
    }
    else if (gComBaudRate == tr("1000000"))
    {
        mBaud = BAUD1000000;
    }
    else if (gComBaudRate == tr("3000000"))
    {
        mBaud = BAUD3000000;
    }


    myCom->setPortName(gComNum);
    myCom->open(QIODevice::ReadWrite);
    if (myCom->isOpen())
    {
        myCom->setBaudRate(mBaud);
        myCom->setDataBits(DATA_8);
        myCom->setFlowControl(FLOW_OFF);
        myCom->setParity(PAR_NONE);
        myCom->setStopBits(STOP_1);
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
    myCom->close();
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
    //arr = pSerialPort->readAll();
    arr = myCom->readAll();

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

    emit serialDataReady();
}


void SerialPortThread::comwrite(QByteArray arr)
{
    if (/*(myCom != nullptr)&&*/(myCom->isOpen()))
    {
        qDebug()<< "[SerialPortThread]com write!!!=="<<ByteArrayToString(arr);

        myCom->write(arr);
    }
    else
    {
        qDebug()<< "[SerialPortThread]pSerialPort is not opened!!!";
    }

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
void SerialPortThread::setBaudRate(QString baudrate)
{
    gComBaudRate = baudrate;
}
