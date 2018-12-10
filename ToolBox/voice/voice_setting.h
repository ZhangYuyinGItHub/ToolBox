#ifndef VOICE_SETTING_H
#define VOICE_SETTING_H

#include <QWidget>
#include <QDialog>
#include <QButtonGroup>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>

class voice_setting : public QDialog
{
    Q_OBJECT
public:
    explicit voice_setting(QWidget *parent = 0, unsigned char type = 0);

private:
    unsigned char gVoiceCodecType;

    typedef struct sbc_param_st
    {
        unsigned short samplingFrequency;
        unsigned short blockNumber;
        unsigned short channelMode;
        unsigned short allocMethod;
        unsigned short subbandNumber;
        unsigned char bitpool;
    }sbc_param, *p_sbc_param;

    /*1. sbc param definition*/
    QGroupBox *pSbcGroup;
    QSpinBox *pBitPoolSpinBox;
    QComboBox *pSamFreqCombox;
    QComboBox *pBlockNumCombox;
    QComboBox *pChanModeCombox;
    QComboBox *pAllocModeCombox;
    QComboBox *pSubBandCombox;

    /*2. MSBC param defination*/
    QGroupBox *pMsbcGroup;

    /*3. adpcm param*/
    QGroupBox *pAdpcmGroup;
    QButtonGroup* pAdpcmBtnGroup;
    int       gAdpcmEncodeType;

public:

    enum{
        ADPCM_16bits_to_4bits_IMA,
        ADPCM_16bits_to_4bits_OPT,
        ADPCM_16bits_to_3bits_OPT_20BYTES,
        ADPCM_16bits_to_3bits_OPT_60BYTES,
    };

    typedef enum{
        SBC_TYPE_CODEC,
        MSBC_TYPE_CODEC,
        ADPCM_TYPE_CODEC,
    }codec_type;

    sbc_param gSbcParam;

    int getVoiceCodedType();
    void show_window(codec_type type);
    void *getSbcParam();
    int getAdpcmParam();

signals:

public slots:
    void voice_adpcm_setting(int id);
    void voice_config_ok();
};

#endif // VOICE_SETTING_H
