#ifndef SBC_H
#define SBC_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QSerialPort>
#include <QLineEdit>
#include "../SerialAssit/qcustomplot.h"
#include <QByteArray>

#include "voice/sbc_lib.h"

class sbc : public QWidget
{
    Q_OBJECT
public:
    explicit sbc(QWidget *parent = nullptr);

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

    Sbc_lib *psbc;

public:
    void audioplay(QString filepath);
    void drawAudioPlot(QString filename);

signals:

public slots:
    void sbc_file_load(void);
    void sbc_file_output(void);

    void pcm_file_load(void);
    void pcm_file_output(void);

    void pcm_2_sbc(void);
    void sbc_2_pcm(void);
};

#endif // SBC_H
