#include "serialassit.h"
#include <QSerialPortInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QSound>
#include <QFileDialog>

SerialAssit::SerialAssit(QWidget *parent) : QWidget(parent)
{
    /*1. 串口参数设置*/
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

    /* 2. 曲线参数设置 */
    gRevDataLen = 0;
    pPlot = new QCustomPlot();
    pPlot->xAxis->setVisible(true);
    pPlot->xAxis2->setVisible(true);
    pPlot->yAxis->setVisible(true);
    pPlot->yAxis2->setVisible(true);
    pPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    /* 页面布局设置 */
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
    pLayout02->addWidget(pPlot);
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
            pComRefreshBtn->setEnabled(false);
        }
    }
    else
    {
        pComNum->setEnabled(true);
        pComBaudRate->setEnabled(true);
        pComRefreshBtn->setEnabled(true);
        pSerialPort->close();
        pComOpenBtn->setText(tr("打开串口"));
    }
}
void SerialAssit::comread()
{

    QByteArray buf;
    buf = pSerialPort->readAll();

    //    if(!buf.isEmpty())
    //    {
    //        QString str = pRevTextEdit->toPlainText();
    //        str+=tr(buf);
    //        pRevTextEdit->clear();
    //        pRevTextEdit->setPlainText(str);
    //    }
    //    buf.clear();
    pPlot->addGraph();
    pPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
    pPlot->graph(0)->setBrush(QBrush(QColor(255, 0, 0, 20)));

    for(int i=0;i<buf.length();i++)
    {
        pPlot->graph(0)->addData(gRevDataLen+i,buf[i]);
        gRevDataLen++;
    }
    //pPlot->graph(1)->setData(x, y);

    pPlot->graph(0)->rescaleAxes();
    pPlot->replot();

    qDebug()<<"gRevDataLen:" + gRevDataLen;
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

#if 1
    {//test
        QFile *inputFile = new QFile();
        QString fileout = QFileDialog::getOpenFileName(this, tr("选择音频文件"), "","*.pcm;;*.*",0);
        inputFile->setFileName(fileout);
        if (false == inputFile->open(QIODevice::ReadOnly))
        {
            qDebug()<<"file open failed";
            return ;
        }

        pPlot->addGraph();
        pPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
        pPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

        QByteArray arr;
        arr = inputFile->readAll();
        quint64 len = inputFile->size();
        for(int i=0;i<len;i+=2)
        {
            //QString str;
            qint32 i0;
            //data0 = (arr[i] + arr[i+1]<<8);

            i0 = arr[i+1];
            i0 = ((i0<<8) + arr[i]);
            if (i0 & 0x8000)
                ;//i0 = 0 - i0;
//            if (i0 > 0)
//                qDebug()<<"<1--->"+ QString::number(i0) ;
//            else
//                qDebug()<<"<-2-->" + QString::number(i0);

            pPlot->graph(0)->addData(i, i0);
        }

        pPlot->graph(0)->rescaleAxes();
        pPlot->replot();

        //设置采样格式
        QAudioFormat audioFormat;
        //设置采样率
        audioFormat.setSampleRate(16000);
        //设置通道数
        audioFormat.setChannelCount(1);
        //设置采样大小，一般为8位或16位
        audioFormat.setSampleSize(16);
        //        //设置编码方式
        audioFormat.setCodec("audio/pcm");
        //        //设置字节序
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        //        //设置样本数据类型
        audioFormat.setSampleType(QAudioFormat::UnSignedInt);

        QAudioOutput *audio = new QAudioOutput( audioFormat, 0);
        if (!audio)
        {
            qDebug()<<"audio open failed";
            return;
        }
        audio->start(inputFile);


    }
#else
    QSound *sound = new QSound("login.wav", this); //构建对象
    sound->play();//播放
    sound->stop();//停止
    sound->setLoops(3);//设置循环次数
#endif
}

