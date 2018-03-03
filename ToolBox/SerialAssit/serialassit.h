#ifndef SERIALASSIT_H
#define SERIALASSIT_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QSerialPort>
#include "qcustomplot.h"

class SerialAssit : public QWidget
{
    Q_OBJECT
public:
    explicit SerialAssit(QWidget *parent = nullptr);

    QSerialPort *pSerialPort;
    QLabel *pComNumLabel;
    QComboBox *pComNum;
    QLabel *pComBaudRateLabel;
    QComboBox *pComBaudRate;
    QPushButton *pComRefreshBtn;
    QPushButton *pComOpenBtn;

    QPlainTextEdit *pRevTextEdit;

    QCustomPlot *pPlot;

    quint64 gRevDataLen;
protected:
    //void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void comfresh(void);
    void comopen(void);
    void comread(void);
};

#endif // SERIALASSIT_H
