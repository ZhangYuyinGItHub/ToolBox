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
#include "database.h"


SerialAssit::SerialAssit(QWidget *parent) : QWidget(parent)
{
    //    this->setStyleSheet("QWidget{background-color: rgb(233, 233, 233);"
    //                      "font: 11pt}"
    //                      "QPushButton{background-color: rgb(199, 199, 199);"
    //                      "border-radius: 3px;"
    //                      "min-height: 18px; "
    //                      "min-width: 80px;  "
    //                      "font: 11pt;"
    //                      "/*border: 1px groove gray;*/}"
    //                      "QPushButton:hover {background-color: lime;}"
    //                      "QPushButton:pressed{background-color:rgb(255, 170, 170);"
    //                                          "border-style: inset;}");

    /*1. 串口参数设置*/
    pComNumLabel = new QLabel(tr("端口号："));
    pComNum = new zComBox();
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
    pSendStartVoice = new QPushButton(/*"开始语音"*/);
    pSendStartVoice->setIcon(QIcon(":/new/prefix2/Image/start.png"));
    pSendStartVoice->setObjectName(QString::number(VOICE_CMD_START, 10));
    pSendStopVoice = new QPushButton(/*"停止语音"*/);
    pSendStopVoice->setIcon(QIcon(":/new/prefix2/Image/stop.png"));
    pSendStopVoice->setObjectName(QString::number(VOICE_CMD_STOP, 10));
    pSendStopVoice->setVisible(false);
    pSendUart2M  = new QPushButton("2M");
    pSendUart2M->setObjectName(QString::number(VOICE_CMD_2M, 10));
    pSettingBtn = new QPushButton();
    pSettingBtn->setIcon(QIcon(":/new/prefix2/Image/setting.png"));
    connect(pSendStartVoice, &QPushButton::released, this, voice_cmd_handler);
    connect(pSendStopVoice, &QPushButton::released, this, voice_cmd_handler);
    connect(pSendUart2M, &QPushButton::released, this, voice_cmd_handler);
    connect(pSettingBtn, &QPushButton::released, this, voice_setting_handler);

    pAudioPlay = new QPushButton(tr("播放语音"));
    pAudioSave = new QPushButton(tr("保存语音"));

    connect(pComOpenBtn,&QPushButton::released,this,comopen);
    connect(pAudioPlay,&QPushButton::released,this,audioplay);
    connect(pAudioSave,&QPushButton::released,this,audiosave);
    connect(pSendBtn,&QPushButton::released,this,comsend);
    connect(pComNum, &zComBox::combox_clicked, this, com_num_brush);

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
    pComBaudRate->setEditable(false);

    /*开启线程串口通讯*/
    pSerialPortThread = new SerialPortThread(nullptr);
    connect(pSerialPortThread, &SerialPortThread::serialDataReady,
            this, &SerialAssit::serialDataRev);
#if DEBUG_EN
    qDebug()<< "main thread id:";
    qDebug()<<QThread::currentThreadId();
#endif

    /* 2. 曲线参数设置 */
    gRevDataLen = 0;
    pPlot = new QCustomPlot();
    pPlot->xAxis->setVisible(true);
    pPlot->xAxis2->setVisible(true);
    pPlot->yAxis->setVisible(true);
    pPlot->yAxis2->setVisible(true);
    pPlot->setInteractions(/*QCP::iRangeDrag |*/ QCP::iRangeZoom);
    pPlot->addGraph();
    pPlot->graph(0)->setPen(QPen(Qt::blue));

    connect(pPlot, &QCustomPlot::mousePress, this, &SerialAssit::show_region_context_menu);

    /* 页面布局设置 */
    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLayout01->addWidget(pComOpenBtn);
    pLayout01->addWidget(pComNum);
    pLayout01->addWidget(pComBaudRate);

    //    pComOpenBtn->setStyleSheet("QPushButton{"
    //                               "background-color: rgb(233, 233, 233, 0);"
    //                               "background-image:url(:/new/prefix2/Image/icon1.png);"//设置按钮背景图片
    //                               "background-repeat:repeat-xy;" //设置按钮的背景图片的重复规则，x方向y方向，xy方向
    //                               "background-position:Center;" //设定背景图片在按钮中的位置,按左对齐Left，右Right，中间Center，上Top，底部Bottom
    //                               "background-attachment:fixed;"
    //                               "min-height: 24px; "
    //                               "min-width: 24px;  "
    //                               "max-height: 24px; "
    //                               "max-width: 24px;  "
    //                               "border-radius: 10px;"
    //                               "background-clip:padding}"
    //                               "QPushButton:hover{background-color:rgb(0, 255, 0);"
    //                               "background-image:url(:/new/prefix2/Image/icon2.png);}");
    pLayout01->addWidget(pSendBtn);
    pLayout01->addWidget(pSendEdit);
    //pLayout01->addStretch(1);

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
    //pLayout02->addLayout(pLayout03);



    QWidget *p = new QWidget();
    QHBoxLayout *pLayout05 = new QHBoxLayout(p);
    pLayout05->addStretch(1);
    pLayout05->addWidget(pSendStartVoice/*, 0, Qt::AlignRight|Qt::AlignTop*/);
    pLayout05->addWidget(pSendStopVoice/*, 0, Qt::AlignRight|Qt::AlignTop*/);
    pLayout05->addWidget(pSendUart2M/*, 0, Qt::AlignRight|Qt::AlignTop*/);
    pLayout05->addWidget(pSettingBtn);

    pLayout05->addStretch(1);

    QVBoxLayout *pLayout04 = new QVBoxLayout(pPlot);
    //pLayout04->addSpacing(13);
    //pLayout04->addWidget(pComOpenBtn, 0, Qt::AlignRight|Qt::AlignTop);
    //pLayout04->addWidget(pAudioPlay, 0, Qt::AlignRight|Qt::AlignTop);
    //pLayout04->addWidget(pAudioSave, 0, Qt::AlignRight|Qt::AlignTop);

    //pLayout04->addStretch(1);
    p->setStyleSheet("QWidget{"
                     "background-color: rgb(233, 233, 233, 0);}"
                     "QPushButton{"
                     "background-color: rgb(233, 233, 233, 255);"
                     "border-radius: 5px;"
                     "min-height: 40px; "
                     "min-width: 40px; }"
                     "QPushButton:hover{"
                     "background-color: rgb(150, 180, 150, 255)}"
                     "QPushButton:pressed{"
                     "background-color:rgb(155, 100, 100);"
                     "border-style: inset;}");
    pLayout04->addWidget(p, 0, Qt::AlignBottom|Qt::AlignCenter);
    pLayout04->addSpacing(10);
    //pLayout04->addStretch(1);
}
/*
 * @brief 曲线右键菜单
*/
void SerialAssit::show_region_context_menu(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QMenu contextMenu(pPlot);

        QAction *pRescale = contextMenu.addAction("RescaleAxes");
        pRescale->setIcon(QIcon(":/new/prefix1/Image/rescale_icon.png"));
        QAction *pRepaly = contextMenu.addAction("Replay");
        pRepaly->setIcon(QIcon(":/new/prefix1/Image/replay_icon.png"));
        QAction *pSave = contextMenu.addAction("save");
        pSave->setIcon(QIcon(":/new/prefix2/Image/save.png"));

        /*
         *step2--事件循环
         */
        QAction *selectaction = contextMenu.exec(event->globalPos());//事件循环

        if (selectaction == pRescale)
        {
            pPlot->graph(0)->rescaleAxes();
            pPlot->replot();//pPlot->graph(0)->data().data()->remove(100*1024);
            pPlot->graph(0)->rescaleAxes();
            pPlot->replot();
        }
        else if (selectaction == pRepaly)
        {
            //audioplay(pAudioInputFile->fileName());
            audioplay();
        }
        else if (selectaction == pSave)
        {
            audiosave();
        }

        //contextMenu.exec(QWidget::pos());
        //不加此句，自定义QGraphicsItem子类无法接收右键事件
        //QWidget::contextMenuEvent(event);
    }
}
void SerialAssit::voice_setting_handler()
{
    qDebug()<<"voice_setting";
    mpDb = new DataBase();//ui->mDataView
    mpDb->InitDataBase();//初始化数据库

    mpDb->InitDeviceView();//初始化数据记录的模型视图

    mpDb->insertRecord("2M",
                       "87 10 11 01 E4 E1",
                       "none");

    mpDb->insertRecord("vStart",
                       "87 05 11 73 75",
                       "none");
    mpDb->insertRecord("vStop",
                       "87 06 11 73 85",
                       "none");



    mpDb->show();

}

void SerialAssit::voice_cmd_handler()
{
    if (!pSerialPortThread->getSerialPortStatus())
    {
        QMessageBox::information(NULL, "Info", tr("串口没有打开"),
                                 QMessageBox::Ok );
        return;
    }

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

            pSendStartVoice->setVisible(false);
            pSendStopVoice->setVisible(true);

            break;
        case VOICE_CMD_STOP:
            cmd =  QString2Hex(voice_cmd_stop);
            pSendStartVoice->setVisible(true);
            pSendStopVoice->setVisible(false);
            break;
        default:break;
        }
        //qDebug()<<"setting button: index = "<< index;

        if (!cmd.isNull())
        {
            pSerialPortThread->comwrite(cmd);
        }
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
#if DEBUG_EN
    qDebug()<<pComOpenBtn->text();
#endif
    if (pComOpenBtn->text() == tr("打开串口"))
    {
        pComNum->setEnabled(false);
        pComBaudRate->setEnabled(false);
        pComOpenBtn->setText(tr("关闭串口"));

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
        //pSerialPort->close();
        pComOpenBtn->setText(tr("打开串口"));

        /*关闭串口通信线程*/
        //pSerialPortThread->exitThread(false);
        pSerialPortThread->exitThread(true);
        if (pSerialPortThread == nullptr)
        {
#if DEBUG_EN
            qDebug() << "pSerialPortThread is null";
#endif
        }
        else
        {
#if DEBUG_EN
            qDebug() << "pSerialPortThread is exit";
#endif
        }

    }
#if DEBUG_EN
    qDebug() << "com open "+QString::number((int)QThread::currentThreadId());
#endif
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
#if DEBUG_EN
        qDebug()<<"[main] send data " + ByteArrayToHexString(arr);
#endif
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
void SerialAssit::com_num_brush()
{
    //qDebug() << "com_num_brush";
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
#if DEBUG_EN
            qDebug()<<"file open failed";
#endif
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
#if DEBUG_EN
            qDebug()<<"audio open failed";
#endif
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

