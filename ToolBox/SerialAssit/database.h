#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QScrollBar>
#include <QDebug>
#include <QSqlRecord>
//#include "mprogressbar.h"
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>

class DataBase : public QWidget
{
    Q_OBJECT
public:
    explicit DataBase(QWidget *parent = 0);
    
public://成员变量
    QTableView *mpView;
    QSqlQueryModel *mpModel;
    //QSqlDatabase db;
    //MProgressBar *mpProgressBar;
    //QProgressBar *mpProgressBar;
    QSqlQueryModel *mpDeviceModel;

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
    bool insertRecord(QString nodeId,
                      QString channel,
                      QString Tvalue,
                      QString time);
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

signals:
    
public slots:
    
};

#endif // DATABASE_H
