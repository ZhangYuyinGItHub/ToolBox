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
    QMessageBox::information(NULL, "SBC", tr("pcm_file_output! "),
                             QMessageBox::Ok );
}

void sbc::pcm_2_sbc(void)
{
    QMessageBox::information(NULL, "SBC", tr("pcm_2_sbc! "),
                             QMessageBox::Ok );
}

void sbc::sbc_2_pcm(void)
{

    QMessageBox::information(NULL, "SBC", tr("sbc_2_pcm--->! "),
                             QMessageBox::Ok );
//    char* inputstr = ("AudioRecord.dat");
//    char* outputstr = ("AudioRecord.wav");

//    voice_sbc(inputstr, outputstr);

}
