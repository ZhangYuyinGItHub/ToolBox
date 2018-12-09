#ifndef VOICE_SETTING_H
#define VOICE_SETTING_H

#include <QWidget>
#include <QDialog>
#include <QButtonGroup>
#include <QGroupBox>

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

    /*sbc param definition*/
    QGroupBox *pSbcGroup;

    sbc_param gSbcParam;

    /*MSBC param defination*/
    QGroupBox *pMsbcGroup;

    /*adpcm param*/
    QGroupBox *pAdpcmGroup;
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

    int getVoiceCodedType();
    void show_window(codec_type type);
    void getSbcParam();

signals:

public slots:
    void voice_adpcm_setting(int id);
    void voice_config_ok();
};

#endif // VOICE_SETTING_H
