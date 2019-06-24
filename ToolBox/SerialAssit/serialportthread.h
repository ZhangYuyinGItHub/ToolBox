#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QString>
#include <QByteArray>

class SerialPortThread: public QObject
{
    Q_OBJECT
public:
    explicit SerialPortThread(QObject *parent);

    QSerialPort *pSerialPort;
    QString gComNum;
    qint32 gComBaudRate;
    QThread *pThread;
    QByteArray gRevBuffer;
    quint32 gRevDataLen;
    //bool  mux ;

    //void run();
    void exitThread(bool sw);
    void setComNum(QString str);
    void setBaudRate(qint32 baudrate);
    void restartThread(void);
    void comwrite(QByteArray arr);
    quint32 getCurrentRevLength(void);
    QByteArray getRevDataArr(int start, int end);
    QString ByteArrayToString(QByteArray &ba);

signals:
    void serialDataReady();


public slots:
    void comread();
    void serialThreadStarted(void);
};

#endif // SERIALPORTTHREAD_H
