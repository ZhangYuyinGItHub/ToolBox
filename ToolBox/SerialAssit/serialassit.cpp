#include "serialassit.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

SerialAssit::SerialAssit(QWidget *parent) : QWidget(parent)
{
    pComNumLabel = new QLabel(tr("端口号："));
    pComNum = new QComboBox();
    pComRefreshBtn = new QPushButton(tr("刷新"));

    connect(pComRefreshBtn,&QPushButton::released,this,comfresh);
    /*search for serial port*/
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            pComNum->addItem(serial.portName());
            serial.close();
        }
    }

    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLayout01->addWidget(pComRefreshBtn);
    pLayout01->addWidget(pComNumLabel);
    pLayout01->addWidget(pComNum);
    pLayout01->addStretch(1);

    QVBoxLayout *pLayout02 = new QVBoxLayout(this);
    pLayout02->addStretch(1);
    pLayout02->addLayout(pLayout01);
}

void SerialAssit::comfresh()
{
    pComNum->clear();
    QStringList pComList;// = new QStringList();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            pComList.append(serial.portName());
            serial.close();
        }
    }
    pComNum->addItems(pComList);
    qDebug()<<"QString::number(offset)";

}

