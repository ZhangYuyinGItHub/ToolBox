#ifndef SERIALASSIT_H
#define SERIALASSIT_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QSerialPort>
#include "qcustomplot.h"
#include "serialportthread.h"
#include <QByteArray>
#include <QFile>
#include <QLineEdit>

class SerialAssit : public QWidget
{
    Q_OBJECT
public:
    explicit SerialAssit(QWidget *parent = nullptr);

    QSerialPort *pSerialPort;
    SerialPortThread *pSerialPortThread;
    QLabel *pComNumLabel;
    QComboBox *pComNum;
    QLabel *pComBaudRateLabel;
    QComboBox *pComBaudRate;
    QPushButton *pComRefreshBtn;
    QPushButton *pComOpenBtn;
    QPushButton *pAudioPlay;
    QPushButton *pAudioSave;

    QPlainTextEdit *pRevTextEdit;
    QLineEdit *pSendEdit;
    QPushButton *pSendBtn;

    QCustomPlot *pPlot;

    quint64 gRevDataLen;
    QByteArray gRevbuf;
    QByteArray gReminderDataArr;

public:
    QByteArray QString2Hex(QString str);
    char ConvertHexChar(char ch);
    QString ByteArrayToHexString(QByteArray &ba);
protected:
    //void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void comfresh(void);
    void comopen(void);
    void comread(void);
    void audioplay(void);
    void audiosave(void);
    void comsend(void);

    void serialDataRev(QByteArray data);
};

#endif // SERIALASSIT_H
