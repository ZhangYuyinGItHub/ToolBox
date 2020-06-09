#include "voice_setting.h"
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
//#include <QDebug>
#include <QObject>
#include <QLabel>
#include "voice.h"

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
        pBitPoolSpinBox = new QSpinBox();//1. bitpool
        pBitPoolSpinBox->setValue(gSbcParam.bitpool);
        pBitPoolSpinBox->setFocusPolicy(Qt::NoFocus);
        pBitPoolSpinBox->setRange(1, 255);

        pSamFreqCombox = new QComboBox();//2. sample freq
        pSamFreqCombox->addItem("Freq 16K");
        pSamFreqCombox->addItem("Freq 32K");
        pSamFreqCombox->addItem("Freq 44.1K");
        pSamFreqCombox->addItem("Freq 48K");
        pSamFreqCombox->setCurrentIndex(gSbcParam.samplingFrequency);

        pBlockNumCombox = new QComboBox();//3. Block number
        pBlockNumCombox->addItem("BLOCKS4");
        pBlockNumCombox->addItem("BLOCKS8");
        pBlockNumCombox->addItem("BLOCKS12");
        pBlockNumCombox->addItem("BLOCKS16");
        pBlockNumCombox->setCurrentIndex(gSbcParam.blockNumber);

        pChanModeCombox = new QComboBox();//4. channel mode
        pChanModeCombox->addItem("MONO");
        pChanModeCombox->addItem("DUAL");
        pChanModeCombox->addItem("STEREO");
        pChanModeCombox->addItem("JOINT");
        pChanModeCombox->setCurrentIndex(gSbcParam.channelMode);

        pAllocModeCombox = new QComboBox();//5. alloc mode
        pAllocModeCombox->addItem("ALLOCLOUDNESS");
        pAllocModeCombox->addItem("ALLOCSNR");
        pAllocModeCombox->setCurrentIndex(gSbcParam.allocMethod);

        pSubBandCombox = new QComboBox();//6. sub band
        pSubBandCombox->addItem("SUBBANDS4");
        pSubBandCombox->addItem("SUBBANDS8");
        pSubBandCombox->setCurrentIndex(gSbcParam.subbandNumber);

        pVLayout02->addWidget(pBitPoolSpinBox);
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

        pAdpcmBtnGroup = new QButtonGroup();
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
        pBox01->setChecked(true);

        connect(pAdpcmBtnGroup,
                static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),
                this,
                &voice_setting::voice_adpcm_setting);
    }

    pOKBtn = new QPushButton("OK");
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

    if (gVoiceCodecType == SBC_TYPE_CODEC)
    {
        gSbcParam.allocMethod       = pAllocModeCombox->currentIndex();
        gSbcParam.bitpool           = pBitPoolSpinBox->value();
        gSbcParam.blockNumber       = pBlockNumCombox->currentIndex();
        gSbcParam.channelMode       = pChanModeCombox->currentIndex();
        gSbcParam.samplingFrequency = pSamFreqCombox->currentIndex();
        gSbcParam.subbandNumber     = pSubBandCombox->currentIndex();

        //qDebug()<< "---sbc---->"<<gSbcParam.samplingFrequency;

    }else if (gVoiceCodecType == MSBC_TYPE_CODEC)
    {

    }else if (gVoiceCodecType == ADPCM_TYPE_CODEC)
    {

    }
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

        //pAdpcmBtnGroup->setId();
    }

    gVoiceCodecType = type;
}

void* voice_setting::getSbcParam()
{
//    param_in->bitpool = gSbcParam.bitpool;
//    param_in->allocMethod = gSbcParam.allocMethod;
//    param_in->blockNumber = gSbcParam.blockNumber;
//    param_in->channelMode = gSbcParam.channelMode;
//    param_in->samplingFrequency = gSbcParam.samplingFrequency;
//    param_in->subbandNumber = gSbcParam.subbandNumber;
    return (void*)&gSbcParam;
}

int voice_setting::getAdpcmParam()
{
    return gAdpcmEncodeType;
}

int voice_setting::getVoiceChalMode()
{
    return gSbcParam.channelMode;
}
int voice_setting::getVoiceSampleRate()
{
    return gSbcParam.samplingFrequency;
}

void voice_setting::setSbcParam(unsigned char* pInputBuffer, int inputSize)
{
    unsigned int c;
    unsigned char *pIn = pInputBuffer;

    if (inputSize < 3)
    {
        return ;
    }

    /* read header */
    c = *pIn++;
    if (c != 0x9C)
    {
        return ;
    }

    c = *pIn++;
    gSbcParam.samplingFrequency = (c >> 6) & 0x03;
    gSbcParam.blockNumber = blockNumbers[(c >> 4) & 0x03];
    gSbcParam.channelMode = (c >> 2) & 0x03;
    gSbcParam.allocMethod = (c >> 1) & 0x01;
    gSbcParam.subbandNumber = subbandNumbers[(c >> 0) & 0x01];
    gSbcParam.bitpool = *pIn++;

    pAllocModeCombox->setCurrentIndex(gSbcParam.allocMethod );
    pBitPoolSpinBox->setValue(gSbcParam.bitpool) ;
    pBlockNumCombox->setCurrentIndex( gSbcParam.blockNumber);
    pChanModeCombox->setCurrentIndex(gSbcParam.channelMode);
    pSamFreqCombox->setCurrentIndex(gSbcParam.samplingFrequency);
    pSubBandCombox->setCurrentIndex(gSbcParam.subbandNumber);

    return ;
}
