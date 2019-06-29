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
#include <QThread>
#include "serialportthread.h"
#include "audiodevice.h"



SerialAssit::SerialAssit(QWidget *parent) : QWidget(parent)
{
    /*1. 串口参数设置*/
    pComNumLabel = new QLabel(tr("端口号："));
    pComNum = new QComboBox();
    pComRefreshBtn = new QPushButton(tr("刷新"));
    pComOpenBtn = new QPushButton(tr("打开串口"));
    pComBaudRateLabel = new QLabel(tr("波特率："));
    pComBaudRate = new QComboBox();
    //pRevTextEdit = new QPlainTextEdit();


    pSendEdit = new QLineEdit();
    pSendBtn = new QPushButton("指令发送");
    pSendEdit->setPlaceholderText("input uart cmd...");
    QRegExp regExp("[a-fA-F0-9 ]{100}");
    pSendEdit->setValidator(new QRegExpValidator(regExp, this));
    pSendEdit->setValidator(new QRegExpValidator(regExp, this));
    pSendStartVoice = new QPushButton("开始语音");
    pSendStartVoice->setObjectName(QString::number(VOICE_CMD_START, 10));
    pSendStopVoice = new QPushButton("停止语音");
    pSendStopVoice->setObjectName(QString::number(VOICE_CMD_STOP, 10));
    pSendUart2M  = new QPushButton("2M");
    pSendUart2M->setObjectName(QString::number(VOICE_CMD_2M, 10));
    connect(pSendStartVoice, &QPushButton::released, this, voice_cmd_handler);
    connect(pSendStopVoice, &QPushButton::released, this, voice_cmd_handler);
    connect(pSendUart2M, &QPushButton::released, this, voice_cmd_handler);

    pAudioPlay = new QPushButton(tr("播放语音"));
    pAudioSave = new QPushButton(tr("保存语音"));

    connect(pComRefreshBtn,&QPushButton::released,this,comfresh);
    connect(pComOpenBtn,&QPushButton::released,this,comopen);
    connect(pAudioPlay,&QPushButton::released,this,audioplay);
    connect(pAudioSave,&QPushButton::released,this,audiosave);
    connect(pSendBtn,&QPushButton::released,this,comsend);

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
    pComBaudRate->addItem("1000000");
    pComBaudRate->addItem("2000000");
    pComBaudRate->addItem("3000000");
    pComBaudRate->setEditable(true);

    /*开启线程串口通讯*/
    pSerialPortThread = new SerialPortThread(nullptr);
    connect(pSerialPortThread, &SerialPortThread::serialDataReady,
            this, &SerialAssit::serialDataRev);

    qDebug()<< "main thread id:";
    qDebug()<<QThread::currentThreadId();

    /* 2. 曲线参数设置 */
    gRevDataLen = 0;
    pPlot = new QCustomPlot();
    pPlot->xAxis->setVisible(true);
    pPlot->xAxis2->setVisible(true);
    pPlot->yAxis->setVisible(true);
    pPlot->yAxis2->setVisible(true);
    pPlot->setInteractions(/*QCP::iRangeDrag |*/ QCP::iRangeZoom);
    pPlot->addGraph();
    pPlot->graph(0)->setPen(QPen(Qt::red));

    /* 页面布局设置 */
    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLayout01->addWidget(pComRefreshBtn);
    pLayout01->addWidget(pComNumLabel);
    pLayout01->addWidget(pComNum);
    pLayout01->addWidget(pComBaudRateLabel);
    pLayout01->addWidget(pComBaudRate);
    pLayout01->addWidget(pComOpenBtn);
    pLayout01->addWidget(pAudioPlay);
    pLayout01->addWidget(pAudioSave);
    pLayout01->addStretch(1);

    QHBoxLayout *pLayout03 = new QHBoxLayout();
    pLayout03->addWidget(pSendBtn);
    pLayout03->addWidget(pSendEdit);
    pLayout03->addWidget(pSendStartVoice);
    pLayout03->addWidget(pSendStopVoice);
    pLayout03->addWidget(pSendUart2M);


    QVBoxLayout *pLayout02 = new QVBoxLayout(this);
    //pLayout02->addStretch(1);
    pLayout02->addWidget(pPlot);
    pLayout02->addLayout(pLayout01);
    pLayout02->addLayout(pLayout03);
}
void SerialAssit::voice_cmd_handler()
{
    QObject *object = QObject::sender();
    QPushButton *push_button = qobject_cast<QPushButton *>(object);
    if (push_button)
    {
        QString object_name = push_button->objectName();
        qint32  index = object_name.toInt();
        QByteArray cmd;
        switch(index)
        {
        case VOICE_CMD_2M:
            cmd =  QString2Hex(voice_cmd_2M);
            break;
        case VOICE_CMD_START:
            cmd =  QString2Hex(voice_cmd_start);
            break;
        case VOICE_CMD_STOP:
            cmd =  QString2Hex(voice_cmd_stop);
            break;
        default:break;
        }

        pSerialPortThread->comwrite(cmd);
    }
}

/**
 * @brief SerialAssit::audioplay
 *        播放语音按钮的槽函数
 */
void SerialAssit::audioplay()
{
    QString fileout = "./AudioFile.pcm";
    QFile *inputFile = new QFile(fileout);

    /*1. read audio data*/
    inputFile->setFileName(fileout);
    if (false == inputFile->open(QIODevice::ReadOnly))
    {
        return ;
    }

    QByteArray audio_data = inputFile->readAll();
    inputFile->close();

    /*2. create audio device*/
    audiodevice *audiodev = new audiodevice(audio_data); //创建自定义的IO设备

    //设置采样格式
    QAudioFormat audioFormat;
    //设置采样率
    audioFormat.setSampleRate(16000);
    //设置通道数
    audioFormat.setChannelCount(1);
    //设置采样大小，一般为8位或16位
    audioFormat.setSampleSize(16);
    //设置编码方式
    audioFormat.setCodec("audio/pcm");
    //设置字节序
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    //设置样本数据类型
    audioFormat.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput *audio = new QAudioOutput( audioFormat, 0);
    if (!audio)
    {
        return;
    }
    audio->start(audiodev);
    //inputFile->close();//打开之后无法播放语音
}
/**
 * @brief SerialAssit::audiosave
 *        保存语音数据
 */
void SerialAssit::audiosave()
{
    if (gRevDataLen == 0)
        return;

    QString fileout = "./AudioFile.pcm";
    QFile *inputFile = new QFile(fileout);
    //QString fileout = QFileDialog::getOpenFileName(this, tr("选择音频文件"), "","*.pcm;;*.*",0);

    inputFile->setFileName(fileout);
    if (false == inputFile->open(QIODevice::WriteOnly))
    {
        return ;
    }

    inputFile->write(gRevbuf);
    inputFile->close();
}
/**
 * @brief SerialAssit::comopen
 *
 * 打开关闭串口按钮的槽函数
 */
void SerialAssit::comopen()
{
    qDebug()<<pComOpenBtn->text();
    if (pComOpenBtn->text() == tr("打开串口"))
    {
        pComNum->setEnabled(false);
        pComBaudRate->setEnabled(false);
        pComOpenBtn->setText(tr("关闭串口"));
        pComRefreshBtn->setEnabled(false);

        pSerialPortThread->setBaudRate(pComBaudRate->currentText());
        pSerialPortThread->setComNum(pComNum->currentText());
        pSerialPortThread->restartThread();

        gRevDataLen = 0;
        gRevbuf.clear();
        pPlot->graph(0)->data().data()->clear();
        pPlot->graph(0)->data().clear();//
        pPlot->graph(0)->rescaleAxes();
        pPlot->replot();
        /*clear buffer data*/
        pPlot->clearGraphs();
        pPlot->addGraph();
        pPlot->graph(0)->setPen(QPen(Qt::red));
    }
    else
    {
        pComNum->setEnabled(true);
        pComBaudRate->setEnabled(true);
        pComRefreshBtn->setEnabled(true);
        //pSerialPort->close();
        pComOpenBtn->setText(tr("打开串口"));

        /*关闭串口通信线程*/
        //pSerialPortThread->exitThread(false);
        pSerialPortThread->exitThread(true);
        if (pSerialPortThread == nullptr)
        {
            qDebug() << "pSerialPortThread is null";
        }
        else
        {
            qDebug() << "pSerialPortThread is exit";
        }

    }

    qDebug() << "com open "+QString::number((int)QThread::currentThreadId());
}
/**
 * @brief SerialAssit::comsend
 *
 * 串口指令发送按钮的槽函数
 */
void SerialAssit::comsend()
{
    QString str = pSendEdit->text();
    QByteArray arr =  QString2Hex(str);

    {
        pSerialPortThread->comwrite(arr);
        qDebug()<<"[main] send data " + ByteArrayToHexString(arr);
    }

}

/**
 * QByteArray convert QString
 *
 *    中间以 “ ” 空格隔开
 */
QString SerialAssit::ByteArrayToHexString(QByteArray &ba)
{
    QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
    QString buf;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每次一个字节的填充到 outchar
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
        buf += str;
    }
    return buf;
}

/**
 * @brief SerialAssit::ConvertHexChar
 *        字符 转 十六进制
 * @param ch
 * @return
 */
char SerialAssit::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

/**
 * @brief SerialAssit::QString2Hex
 *        QString 字符串转 Hex 格式
 * @param 输入的str
 * @return Hex
 */
QByteArray SerialAssit::QString2Hex(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

/**
 * @brief SerialAssit::comfresh
 *        刷新可以使用的串口号
 */
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

    return;

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
        //inputFile->close();

    }
#else
    //    QSound *sound = new QSound("login.wav", this); //构建对象
    //    sound->play();//播放
    //    sound->stop();//停止
    //    sound->setLoops(3);//设置循环次数
#endif
}

/**
 * @brief SerialAssit::serialDataRev
 *        语音数据接受槽函数，接受来自串口读取线程的语音数据，并进行绘图
 * @param data 语音数据
 */
void SerialAssit::serialDataRev()
{

    quint32 length = pSerialPortThread->getCurrentRevLength();
    quint32 length0;

    if ((length == 0)||(length <= gRevDataLen))
        return;


    QByteArray arr = pSerialPortThread->getRevDataArr(gRevDataLen, length);
    length0 = arr.length();

    //for (;length > gRevDataLen; )
    {
        for (quint32 index = 0  /*- gRevDataLen%2 */;
             index < length0; index += 2)
        {

            qint16 i0;
            i0 = arr[index+1];
            i0 = (i0<<8) | (arr[index]&0xff);
            //qDebug()<< QString::number(i0,16);
            pPlot->graph(0)->addData((index + gRevDataLen)/2, i0);
        }

        gRevDataLen += length0;
        gRevbuf.append(arr);

        pPlot->graph(0)->rescaleAxes();
        pPlot->replot();
    }

#if 0
    //gRevbuf.append(data);

    quint32 len= data.length();
    len = len - len % 2;

    for(quint32 i = (gRevDataLen - gRevDataLen % 2); i < (gRevDataLen + len -2); i += 2)
    {
        qint16 i0;
        i0 = gRevbuf[i+1];
        i0 = ((i0<<8) | (gRevbuf[i]&0xff));
        pPlot->graph(0)->addData(i/2, i0);
    }
    pPlot->graph(0)->rescaleAxes();
    pPlot->replot();

    gRevDataLen = gRevDataLen + len;
#endif
}

