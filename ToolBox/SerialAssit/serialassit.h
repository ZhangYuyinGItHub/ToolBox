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

    QCustomPlot *pPlot;

    quint64 gRevDataLen;
    QByteArray gRevbuf;
    QByteArray gReminderDataArr;
protected:
    //void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void comfresh(void);
    void comopen(void);
    void comread(void);
    void audioplay(void);
    void audiosave(void);

    void serialDataRev(QByteArray data);
};

#endif // SERIALASSIT_H
