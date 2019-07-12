#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QString>
#include <QByteArray>

#include "qextserialport.h"

class SerialPortThread: public QObject
{
    Q_OBJECT
public:
    explicit SerialPortThread(QObject *parent);

    //QSerialPort *pSerialPort;
    QString gComNum;
    QString gComBaudRate;
    QThread *pThread;
    QByteArray gRevBuffer;
    quint32 gRevDataLen;
    //bool  mux ;

    Win_QextSerialPort *myCom;

    //void run();
    void exitThread();
    void setComNum(QString str);
    void setBaudRate(QString baudrate);
    void restartThread(void);
    void comwrite(QByteArray arr);
    quint32 getCurrentRevLength(void);
    QByteArray getRevDataArr(int start, int end);
    QString ByteArrayToString(QByteArray &ba);
    boolean getSerialPortStatus(void);

signals:
    void serialDataReady();


public slots:
    void comread();
    void serialThreadStarted(void);
};

#endif // SERIALPORTTHREAD_H
