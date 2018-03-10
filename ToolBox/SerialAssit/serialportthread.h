#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QString>

class SerialPortThread: public QObject
{
    Q_OBJECT
public:
    explicit SerialPortThread(QObject *parent);

    QSerialPort *pSerialPort;
    bool gThreadSwitch;
    QString gComNum;
    qint32 gComBaudRate;
    QThread *pThread;

    void run();
    void exitThread(bool sw);
    void setComNum(QString str);
    void setBaudRate(qint32 baudrate);
    void restartThread(void);

signals:
    void serialDataReady(QByteArray data);

public slots:
    void comread();
};

#endif // SERIALPORTTHREAD_H
