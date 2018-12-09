#include "voice_setting.h"
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QObject>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>

voice_setting::voice_setting(QWidget *parent, unsigned char type) : QDialog(parent)
{
    QVBoxLayout *pVLayout1 = new QVBoxLayout();

    /*1. SBC Config*/
    pSbcGroup = new QGroupBox("[SBC Setting]");
    {
        QHBoxLayout *pHLayout01 = new QHBoxLayout();

        gSbcParam.bitpool           = 14;
        gSbcParam.samplingFrequency = 0x00;
        gSbcParam.blockNumber       = 0x03;
        gSbcParam.channelMode       = 0x00;
        gSbcParam.allocMethod       = 0x00;
        gSbcParam.subbandNumber     = 0x01;

        /*param name*/
        QVBoxLayout *pVLayout01 = new QVBoxLayout();
        QLabel *pBitPoolLabel = new QLabel("Bit Pool:");
        QLabel *pSamFreqLabel = new QLabel("Sample Freq:");
        QLabel *pBlockNumLabel = new QLabel("Block Num:");
        QLabel *pChannelModeLabel = new QLabel("Channel Mode:");
        QLabel *pAllocModeLabel = new QLabel("Alloc Mode:");
        QLabel *pSubBandNumLabel = new QLabel("SubBand Num:");

        pVLayout01->addWidget(pBitPoolLabel);
        pVLayout01->addWidget(pSamFreqLabel);
        pVLayout01->addWidget(pBlockNumLabel);
        pVLayout01->addWidget(pChannelModeLabel);
        pVLayout01->addWidget(pAllocModeLabel);
        pVLayout01->addWidget(pSubBandNumLabel);

        /*param value*/
        QVBoxLayout *pVLayout02 = new QVBoxLayout();
        QSpinBox *pSpinBox = new QSpinBox();//1. bitpool
        pSpinBox->setValue(gSbcParam.bitpool);
        pSpinBox->setFocusPolicy(Qt::NoFocus);
        pSpinBox->setRange(1, 255);

        QComboBox *pSamFreqCombox = new QComboBox();//2. sample freq
        pSamFreqCombox->addItem("Freq 16K");
        pSamFreqCombox->addItem("Freq 32K");
        pSamFreqCombox->addItem("Freq 44.1K");
        pSamFreqCombox->addItem("Freq 48K");
        pSamFreqCombox->setCurrentIndex(gSbcParam.samplingFrequency);

        QComboBox *pBlockNumCombox = new QComboBox();//3. Block number
        pBlockNumCombox->addItem("BLOCKS4");
        pBlockNumCombox->addItem("BLOCKS8");
        pBlockNumCombox->addItem("BLOCKS12");
        pBlockNumCombox->addItem("BLOCKS16");
        pBlockNumCombox->setCurrentIndex(gSbcParam.blockNumber);

        QComboBox *pChanModeCombox = new QComboBox();//4. channel mode
        pChanModeCombox->addItem("MONO");
        pChanModeCombox->addItem("DUAL");
        pChanModeCombox->addItem("STEREO");
        pChanModeCombox->addItem("JOINT");
        pChanModeCombox->setCurrentIndex(gSbcParam.channelMode);

        QComboBox *pAllocModeCombox = new QComboBox();//5. alloc mode
        pAllocModeCombox->addItem("ALLOCLOUDNESS");
        pAllocModeCombox->addItem("ALLOCSNR");
        pAllocModeCombox->setCurrentIndex(gSbcParam.allocMethod);

        QComboBox *pSubBandCombox = new QComboBox();//6. sub band
        pSubBandCombox->addItem("SUBBANDS4");
        pSubBandCombox->addItem("SUBBANDS8");
        pSubBandCombox->setCurrentIndex(gSbcParam.subbandNumber);

        pVLayout02->addWidget(pSpinBox);
        pVLayout02->addWidget(pSamFreqCombox);
        pVLayout02->addWidget(pBlockNumCombox);
        pVLayout02->addWidget(pChanModeCombox);
        pVLayout02->addWidget(pAllocModeCombox);
        pVLayout02->addWidget(pSubBandCombox);


        pHLayout01->addLayout(pVLayout01);
        pHLayout01->addLayout(pVLayout02);

        pSbcGroup->setLayout(pHLayout01);

    }
    /*2. MSBC Config*/

    /*3. ADPCM Config*/
    pAdpcmGroup = new QGroupBox("[ADPCM Setting]");
    {

        QVBoxLayout *pVLayout01 = new QVBoxLayout();

        QButtonGroup* pAdpcmBtnGroup = new QButtonGroup();
        QCheckBox *pBox01 = new QCheckBox("ADPCM 16bits to 4bits (IMA)", this);
        QCheckBox *pBox02 = new QCheckBox("ADPCM 16bits to 4bits (OPT)", this);
        QCheckBox *pBox03 = new QCheckBox("ADPCM 16bits to 3bits (OPT 20Bytes)", this);
        QCheckBox *pBox04 = new QCheckBox("ADPCM 16bits to 3bits (OPT 60Bytes)", this);

        pVLayout01->addWidget(pBox01);
        pVLayout01->addWidget(pBox02);
        pVLayout01->addWidget(pBox03);
        pVLayout01->addWidget(pBox04);
        pVLayout01->addStretch();

        pAdpcmGroup->setLayout(pVLayout01);


        pAdpcmBtnGroup->addButton(pBox01,0);
        pAdpcmBtnGroup->addButton(pBox02,1);
        pAdpcmBtnGroup->addButton(pBox03,2);
        pAdpcmBtnGroup->addButton(pBox04,3);

        connect(pAdpcmBtnGroup,
                static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),
                this,
                &voice_setting::voice_adpcm_setting);
    }

    QPushButton *pOKBtn = new QPushButton("OK");
    connect(pOKBtn, &QPushButton::clicked,this, &voice_setting::voice_config_ok);

    pVLayout1->addWidget(pSbcGroup);
    pVLayout1->addWidget(pAdpcmGroup);
    pVLayout1->addWidget(pOKBtn);
    this->setLayout(pVLayout1);


    this->setModal(true);

    gVoiceCodecType = type;
    if (gVoiceCodecType == SBC_TYPE_CODEC)
    {
        pSbcGroup->setVisible(true);
        pAdpcmGroup->setVisible(false);
    }else if (gVoiceCodecType == MSBC_TYPE_CODEC)
    {
        pSbcGroup->setVisible(false);
        pAdpcmGroup->setVisible(false);
    }else if (gVoiceCodecType == ADPCM_TYPE_CODEC)
    {
        pSbcGroup->setVisible(false);
        pAdpcmGroup->setVisible(true);
    }

    gAdpcmEncodeType = ADPCM_16bits_to_4bits_IMA;
}

void voice_setting::voice_adpcm_setting(int id)
{
    //qDebug()<< "clicked id = "<<id;
    switch(id)
    {
    case 0:
        gAdpcmEncodeType = ADPCM_16bits_to_4bits_IMA;
        break;
    case 1:
        gAdpcmEncodeType = ADPCM_16bits_to_4bits_OPT;
        break;
    case 2:
        gAdpcmEncodeType = ADPCM_16bits_to_3bits_OPT_20BYTES;
        break;
    case 3:
        gAdpcmEncodeType = ADPCM_16bits_to_3bits_OPT_60BYTES;
        break;
    default:
        break;
    }


}

int voice_setting::getVoiceCodedType()
{
    return gAdpcmEncodeType;
}

void voice_setting::voice_config_ok()
{
    this->setVisible(false);
}

void voice_setting::show_window(codec_type type)
{
    this->setVisible(true);

    if (type == SBC_TYPE_CODEC)
    {
        pSbcGroup->setVisible(true);
        pAdpcmGroup->setVisible(false);
    }else if (type == MSBC_TYPE_CODEC)
    {
        pSbcGroup->setVisible(false);
        pAdpcmGroup->setVisible(false);
    }else if (type == ADPCM_TYPE_CODEC)
    {
        pSbcGroup->setVisible(false);
        pAdpcmGroup->setVisible(true);
    }
}

void voice_setting::getSbcParam()
{
//    param_in->bitpool = gSbcParam.bitpool;
//    param_in->allocMethod = gSbcParam.allocMethod;
//    param_in->blockNumber = gSbcParam.blockNumber;
//    param_in->channelMode = gSbcParam.channelMode;
//    param_in->samplingFrequency = gSbcParam.samplingFrequency;
//    param_in->subbandNumber = gSbcParam.subbandNumber;
    //return &gSbcParam;
}
