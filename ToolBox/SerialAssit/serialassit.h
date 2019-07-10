#ifndef SERIALASSIT_H
#define SERIALASSIT_H

/**
  * @note  本功能的实现使用QSerialPort类，但是有个问题是当窗口拖动的时候会出现丢数据的问题；
  *        可以后续尝试使用第三方库 Qextserialport
  */

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QSerialPort>
#include "qcustomplot.h"
#include "serialportthread.h"
#include <QByteArray>
#include <QFile>
#include <QLineEdit>
#include "win_qextserialport.h"
#include "zcombox.h"
#include "database.h"

const QString voice_cmd_2M = QString("87 10 11 01 E4 E1 ");
const QString voice_cmd_start = QString("87 05 11 73 75");
const QString voice_cmd_stop = QString("87 06 11 73 85");

class SerialAssit : public QWidget
{
    Q_OBJECT
public:
    explicit SerialAssit(QWidget *parent = nullptr);

    SerialPortThread *pSerialPortThread;
    QLabel *pComNumLabel;
    zComBox *pComNum;
    QLabel *pComBaudRateLabel;
    QComboBox *pComBaudRate;
    QPushButton *pComOpenBtn;
    QPushButton *pAudioPlay;
    QPushButton *pAudioSave;

    QPlainTextEdit *pRevTextEdit;
    QLineEdit *pSendEdit;
    QPushButton *pSendBtn;
    QPushButton *pSendUart2M;
    QPushButton *pSendStartVoice;
    QPushButton *pSendStopVoice;
    QPushButton *pSettingBtn;

    QCustomPlot *pPlot;

    quint32 gRevDataLen;
    QByteArray gRevbuf;

    enum
    {
        VOICE_CMD_2M = 0,
        VOICE_CMD_START = 1,
        VOICE_CMD_STOP = 2,
        VOICE_SETTING = 3,
    };

    /*
     *数据页--数据库对象
     */
    DataBase *mpDb;
    QTableView *mCmdTable;

public:
    QByteArray QString2Hex(QString str);
    char ConvertHexChar(char ch);
    QString ByteArrayToHexString(QByteArray &ba);
protected:
    //void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void comopen(void);
    void audioplay(void);
    void audiosave(void);
    void comsend(void);
    void voice_cmd_handler(void);
    void voice_setting_handler(void);

    void serialDataRev();
    void com_num_brush();

    void show_region_context_menu(QMouseEvent *event);
};

#endif // SERIALASSIT_H
