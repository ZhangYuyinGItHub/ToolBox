#include "serialassit.h"
#include <QSerialPortInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QByteArray>

SerialAssit::SerialAssit(QWidget *parent) : QWidget(parent)
{
    pComNumLabel = new QLabel(tr("端口号："));
    pComNum = new QComboBox();
    pComRefreshBtn = new QPushButton(tr("刷新"));
    pComOpenBtn = new QPushButton(tr("打开串口"));
    pComBaudRateLabel = new QLabel(tr("波特率："));
    pComBaudRate = new QComboBox();
    pRevTextEdit = new QPlainTextEdit();

    pSerialPort = new QSerialPort();

    connect(pComRefreshBtn,&QPushButton::released,this,comfresh);
    connect(pComOpenBtn,&QPushButton::released,this,comopen);
    connect(pSerialPort,&QSerialPort::readyRead,this,comread);
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

    pComBaudRate->addItem("9600");
    pComBaudRate->addItem("115200");
    pComBaudRate->addItem("2000000");
    pComBaudRate->addItem("3000000");
    pComBaudRate->setEditable(true);

    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLayout01->addWidget(pComRefreshBtn);
    pLayout01->addWidget(pComNumLabel);
    pLayout01->addWidget(pComNum);
    pLayout01->addWidget(pComBaudRateLabel);
    pLayout01->addWidget(pComBaudRate);
    pLayout01->addWidget(pComOpenBtn);
    pLayout01->addStretch(1);

    QVBoxLayout *pLayout02 = new QVBoxLayout(this);
    //pLayout02->addStretch(1);
    pLayout02->addWidget(pRevTextEdit);
    pLayout02->addLayout(pLayout01);
}
void SerialAssit::comopen()
{
    qDebug()<<pComOpenBtn->text();
    if (pComOpenBtn->text() == tr("打开串口"))
    {
        pSerialPort->setPortName(pComNum->currentText());
        if (pSerialPort->open(QIODevice::ReadWrite))
        {
            pSerialPort->setBaudRate(pComBaudRate->currentText().toInt());
            pSerialPort->setDataBits(QSerialPort::Data8);
            pSerialPort->setParity(QSerialPort::NoParity);
            pSerialPort->setStopBits(QSerialPort::OneStop);
            pSerialPort->setFlowControl(QSerialPort::NoFlowControl);

            pComNum->setEnabled(false);
            pComBaudRate->setEnabled(false);
            pComOpenBtn->setText(tr("关闭串口"));
        }
    }
    else
    {
        pComNum->setEnabled(true);
        pComBaudRate->setEnabled(true);
        pSerialPort->close();
        pComOpenBtn->setText(tr("打开串口"));
    }
}
void SerialAssit::comread()
{
    qDebug()<<"read ready";
    QByteArray buf;
    buf = pSerialPort->readAll();
    if(!buf.isEmpty())
    {
        QString str = pRevTextEdit->toPlainText();
        str+=tr(buf);
        pRevTextEdit->clear();
        pRevTextEdit->setPlainText(str);
    }
    buf.clear();
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

