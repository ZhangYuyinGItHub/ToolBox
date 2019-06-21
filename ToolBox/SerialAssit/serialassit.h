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


const QString voice_cmd_2M = QString("87 10 11 01 E4 E1 ");
const QString voice_cmd_start = QString("87 05 11 73 75");
const QString voice_cmd_stop = QString("87 06 11 73 85");

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
    QPushButton *pSendUart2M;
    QPushButton *pSendStartVoice;
    QPushButton *pSendStopVoice;

    QCustomPlot *pPlot;

    quint64 gRevDataLen;
    QByteArray gRevbuf;

    enum
    {
        VOICE_CMD_2M,
        VOICE_CMD_START,
        VOICE_CMD_STOP,
    };

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
    void audioplay(void);
    void audiosave(void);
    void comsend(void);
    void voice_cmd_handler(void);

    void serialDataRev(uint64_t length0);
};

#endif // SERIALASSIT_H
