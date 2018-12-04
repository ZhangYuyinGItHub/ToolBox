#ifndef SBC_H
#define SBC_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QSerialPort>
#include <QLineEdit>
#include "../SerialAssit/qcustomplot.h"
#include <QByteArray>
#include <QAudioOutput>
#include <QContextMenuEvent>

#include "voice/sbc_lib.h"
#include "voice/msbc_lib.h"

class voice : public QWidget
{
    Q_OBJECT
public:
    explicit voice(QWidget *parent = nullptr);

    QCustomPlot *pPlot;
    QPushButton *pSbcFileLoadBtn;
    QPushButton *pPcmFileOutBtn ;
    QLineEdit   *pSbcInFilePath;
    QLineEdit   *pPcmOutFilePath;
    QPushButton *pSbc2PcmBtn;

    QPushButton *pPcmFileLoadBtn;
    QPushButton *pSbcFileOutBtn ;
    QLineEdit   *pSbcOutFilePath;
    QLineEdit   *pPcmInFilePath;
    QPushButton *pPcm2SbcBtn;

    QGroupBox *pSbc2PcmGroup;
    QGroupBox *pPcm2SbcGroup;

    quint8  mAudioCodecMode;

    Sbc_lib *psbc;
    Msbc_lib *pmsbc;
    QFile *pAudioInputFile;
    QAudioOutput *audio;

protected:
    void contextMenuEvent(QContextMenuEvent *event);

public:
    void audioplay(QString filepath);
    void drawAudioPlot(QString filename);

signals:

public slots:
    void sbc_file_load(void);
    void coded_file_output(void);

    void pcm_file_load(void);
    void pcm_file_output(void);

    void pcm_2_sbc(void);
    void codec_2_pcm(void);

    void show_region_context_menu(QMouseEvent*event);
};

#endif // SBC_H
