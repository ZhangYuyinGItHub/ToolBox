#ifndef SERIALASSIT_H
#define SERIALASSIT_H

#define DEBUG_EN  0

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
#include <QMap>

//const QString voice_cmd_2M = QString("87 10 11 01 E4 E1 ");
//const QString voice_cmd_start = QString("87 05 11 73 75");
//const QString voice_cmd_stop = QString("87 06 11 73 85");

class SerialAssit : public QWidget
{
    Q_OBJECT
public:
    explicit SerialAssit(QWidget *parent = nullptr);

    SerialPortThread *pSerialPortThread;

    zComBox *pComNum;
    QComboBox *pComBaudRate;
    QPushButton *pComOpenBtn;
    QLineEdit *pSendEdit;
    QPushButton *pSendBtn;
    QPushButton *pSendUart2M;
    QPushButton *pSendStartVoice;
    QPushButton *pSendStopVoice;

    QPushButton *pSettingBtn;

    QCustomPlot *pPlot;

    quint32 gRevDataLen;
    QByteArray gRevbuf;

    QLabel *pLabel;
    QLabel *pSizeLabel;
    enum
    {
        VOICE_CMD_2M = 0,
        VOICE_CMD_START = 1,
        VOICE_CMD_STOP = 2,
        VOICE_CMD_USER0 = 3,
        VOICE_CMD_USER1 = 4,
    };

    typedef enum
    {
        SYS_STATE_IDLE = 0,
        SYS_STATE_VOICE_START = 1,
        SYS_STATE_VOICE_STOP = 2,
        SYS_STATE_CMD = 3,
    }sys_state;
    sys_state gSysState,gPreSysState;

    QMap<quint8, QString> voice_cmd_map;
    /*
     *数据页--数据库对象
     */
    DataBase *mpDb;

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

    void setting_window_ok_pressed();
};

#endif // SERIALASSIT_H
