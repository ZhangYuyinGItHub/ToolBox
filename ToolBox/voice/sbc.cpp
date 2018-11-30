#include "sbc.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QSound>
#include <QFileDialog>
#include <QGroupBox>
#include <QMessageBox>

sbc::sbc(QWidget *parent) : QWidget(parent)
{
    /* 1. 曲线参数设置 */
    pPlot = new QCustomPlot();
    pPlot->xAxis->setVisible(true);
    pPlot->xAxis2->setVisible(true);
    pPlot->yAxis->setVisible(true);
    pPlot->yAxis2->setVisible(true);
    pPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    pPlot->addGraph();
    pPlot->graph(0)->setPen(QPen(Qt::red));

    /*SBC --> PCM*/
    QGroupBox *pSbc2PcmGroup = new QGroupBox("SBC-->PCM");
    {
        QVBoxLayout *pVLayout = new QVBoxLayout();
        QHBoxLayout *pHLayout01 = new QHBoxLayout();
        QHBoxLayout *pHLayout02 = new QHBoxLayout();

        pSbcFileLoadBtn = new QPushButton("load sbc");
        pPcmFileOutBtn = new QPushButton("output file");

        pSbcInFilePath = new QLineEdit();
        pPcmOutFilePath = new QLineEdit();

        pSbc2PcmBtn = new QPushButton("convert");
        pSbc2PcmBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

        pHLayout01->addWidget(pSbcFileLoadBtn);
        pHLayout01->addWidget(pSbcInFilePath);

        pHLayout02->addWidget(pPcmFileOutBtn);
        pHLayout02->addWidget(pPcmOutFilePath);

        pVLayout->addLayout(pHLayout01);
        pVLayout->addLayout(pHLayout02);


        QHBoxLayout *pHLayout00 = new QHBoxLayout();
        pHLayout00->addLayout(pVLayout);
        pHLayout00->addWidget(pSbc2PcmBtn);
        pSbc2PcmGroup->setLayout(pHLayout00);

        connect(pSbcFileLoadBtn, &QPushButton::released, this, &sbc::sbc_file_load);
        connect(pPcmFileOutBtn, &QPushButton::released, this, &sbc::pcm_file_output);
        connect(pSbc2PcmBtn, &QPushButton::released, this, &sbc::sbc_2_pcm);
    }

    /*PCM --> SBC 页面设置*/
    QGroupBox *pPcm2SbcGroup = new QGroupBox("PCM-->SBC");
    {
        QVBoxLayout *pVLayout = new QVBoxLayout();
        QHBoxLayout *pHLayout01 = new QHBoxLayout();
        QHBoxLayout *pHLayout02 = new QHBoxLayout();

        pPcmFileLoadBtn = new QPushButton("load pcm");
        pSbcFileOutBtn = new QPushButton("output file");

        pPcm2SbcBtn = new QPushButton("convert");
        pPcm2SbcBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

        pPcmInFilePath = new QLineEdit();
        pSbcOutFilePath = new QLineEdit();

        pHLayout01->addWidget(pPcmFileLoadBtn);
        pHLayout01->addWidget(pPcmInFilePath);

        pHLayout02->addWidget(pSbcFileOutBtn);
        pHLayout02->addWidget(pSbcOutFilePath);

        pVLayout->addLayout(pHLayout01);
        pVLayout->addLayout(pHLayout02);

        QHBoxLayout *pHLayout00 = new QHBoxLayout();
        pHLayout00->addLayout(pVLayout);
        pHLayout00->addWidget(pPcm2SbcBtn);
        pSbc2PcmGroup->setLayout(pHLayout00);
        pPcm2SbcGroup->setLayout(pHLayout00);

        /*消息设置*/
        connect(pPcmFileLoadBtn, &QPushButton::released, this, &sbc::pcm_file_load);
        connect(pSbcFileOutBtn, &QPushButton::released, this, &sbc::sbc_file_output);
        connect(pPcm2SbcBtn, &QPushButton::released, this, &sbc::pcm_2_sbc);
    }

    /* 页面布局设置 */
    QVBoxLayout *pLayout02 = new QVBoxLayout(this);
    //pLayout02->addStretch(1);
    pLayout02->addWidget(pPlot, 1);
    pLayout02->addWidget(pSbc2PcmGroup, 0);
    pLayout02->addWidget(pPcm2SbcGroup, 0);
}

void sbc::sbc_file_load(void)
{
    QMessageBox::information(NULL, "SBC", tr("sbc_file_load! "),
                             QMessageBox::Ok );

    QString fileout = QFileDialog::getOpenFileName(this, tr("加载SBC文件"), "","*.dat",0);
    if (!fileout.isNull())
    {
        pSbcInFilePath->setText(fileout);
    }
}

void sbc::sbc_file_output(void)
{
    QMessageBox::information(NULL, "SBC", tr("sbc_file_output! "),
                             QMessageBox::Ok );
}
void sbc::pcm_file_load(void)
{
    QMessageBox::information(NULL, "SBC", tr("pcm_file_load! "),
                             QMessageBox::Ok );
}
void sbc::pcm_file_output(void)
{
//    QMessageBox::information(NULL, "SBC", tr("pcm_file_output! "),
//                             QMessageBox::Ok );

    QString fileout = QFileDialog::getOpenFileName(this, tr("输出PCM文件"), "","*.wav",0);
    if (!fileout.isNull())
    {
        pPcmOutFilePath->setText(fileout);
    }
}

void sbc::pcm_2_sbc(void)
{
//    QMessageBox::information(NULL, "SBC", tr("pcm_2_sbc! "),
//                             QMessageBox::Ok );

    pmsbc = new Msbc_lib();
    pmsbc->msbc_decoder(pSbcInFilePath->text().toLatin1().data(),
                        pPcmOutFilePath->text().toLatin1().data());

    drawAudioPlot(pPcmOutFilePath->text());
    audioplay(pPcmOutFilePath->text());
}

void sbc::sbc_2_pcm(void)
{

//    QMessageBox::information(NULL, "SBC", tr("sbc_2_pcm--->! "),
//                             QMessageBox::Ok );

    psbc = new Sbc_lib();

    psbc->sbc_encode(pSbcInFilePath->text().toLatin1().data(),
                     pPcmOutFilePath->text().toLatin1().data());

    drawAudioPlot(pPcmOutFilePath->text());
    audioplay(pPcmOutFilePath->text());


}

void sbc::drawAudioPlot(QString filename)
{
    QFile *inputFile = new QFile(filename);

    inputFile->setFileName(filename);
    if (false == inputFile->open(QIODevice::ReadOnly))
    {
        return ;
    }


    QByteArray arr = inputFile->readAll();

//    QVector <double> data;
//    QVector <double> i;
    for (int index = 0; index < arr.size(); index = index + 2)
    {
        qint32 i0 = 0;
        i0 = arr[index+1];
        i0 = ((i0<<8) | arr[index]);
        pPlot->graph(0)->addData(index/2, i0);
    }


    pPlot->graph(0)->rescaleAxes();
    pPlot->replot();

    inputFile->close();
}

void sbc::audioplay(QString filepath)
{
    QFile *inputFile = new QFile(filepath);

    inputFile->setFileName(filepath);
    if (false == inputFile->open(QIODevice::ReadOnly))
    {
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
    audio->start(inputFile);

    //inputFile->close();//打开之后无法播放语音
}
