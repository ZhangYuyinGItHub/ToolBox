#include "voice_setting.h"
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QObject>

voice_setting::voice_setting(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *pVLayout1 = new QVBoxLayout();

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

    pVLayout1->addWidget(pAdpcmGroup);
    this->setLayout(pVLayout1);
    QPushButton *pOKBtn = new QPushButton("OK");
    connect(pOKBtn, &QPushButton::clicked,this, &voice_setting::voice_config_ok);
    pVLayout1->addWidget(pOKBtn);


    this->setModal(true);
    //this->setFixedSize(320, 215);

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
