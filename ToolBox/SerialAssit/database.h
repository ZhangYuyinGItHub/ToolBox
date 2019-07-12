#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QScrollBar>
#if DEBUG_EN
#include <QDebug>
#endif
#include <QSqlRecord>
//#include "mprogressbar.h"
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>
#include <QModelIndex>
#include <QLineEdit>

class DataBase : public QDialog
{
    Q_OBJECT
public:
    explicit DataBase(QDialog *parent = 0);
    ~DataBase();
    
public://成员变量
    QTableView *mpView;
    QSqlQueryModel *mpModel;

    QLineEdit* pName;
    QLineEdit* pCmd;
    QLineEdit* pNote;


public://成员函数
    int InitDataBase(void);
    void InitDeviceView();//初始化设备视图
    void InitDataView(QTableView *mpDataView);//初始化温度记录视图

    int GetTableCount();
    int ExportExcel(QString fileName,
                    QString TableName,
                    QString SheetName);
    bool RemoveConnection();
    int createConnection();//创建数据库链接
    void timeSearch(QString time);
    void nodeSearch(QString node, int channel);
    void ScreenDataBase();
    QSqlRecord getRecord(int row);
    void timeNodeSearch(QString time,
                        QString node,
                        int channel);

    long getRecordNum(QString tablename);//获取数据库的record数目
    bool insertRecord(QString name,
                      QString cmd,
                      QString note);
    void insertDeviceRecord(QTableView *mpDeviceView,
                                      QString deviceId,
                                      QString nodename,
                                      QString nodearea,
                                      QString alertTvalue,
                                      QString sampleinterval,
                                      QString channelname);
    void insertDeviceRecord(QSqlRecord mRecord);
    void updateDeviceView(QString deviceId);
    QSqlRecord getDeviceRecord(QString deviceId);

    void nameSearch(QString time);
    QString getVoice2MCmd();
    QString getVoiceStartCmd();
    QString getVoiceStopCmd();

signals:
    void ok_pressed();
public slots:
    void cmd_doubleclicked(const QModelIndex &index);
    void ok_btn_clicked();
    void cancel_btn_clicked();
    void update_btn_clicked();
};

#endif // DATABASE_H
