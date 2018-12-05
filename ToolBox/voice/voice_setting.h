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
    explicit voice_setting(QWidget *parent = 0);

private:
    QGroupBox *pAdpcmGroup;
    int       gAdpcmEncodeType;

public:
    enum{
        ADPCM_16bits_to_4bits_IMA,
        ADPCM_16bits_to_4bits_OPT,
        ADPCM_16bits_to_3bits_OPT_20BYTES,
        ADPCM_16bits_to_3bits_OPT_60BYTES,
    };


    int getVoiceCodedType();
signals:

public slots:
    void voice_adpcm_setting(int id);
    void voice_config_ok();
};

#endif // VOICE_SETTING_H
