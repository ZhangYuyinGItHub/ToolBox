#include "database.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QPushButton>
#include <QIcon>

DataBase::DataBase(QDialog *parent) :
    QDialog(parent)
{
    //this->parent = parent;
    //数据列表视图
    mpView = new QTableView();
    mpModel = new QSqlQueryModel(this);
    mpView->setModel(mpModel);

    QVBoxLayout *pLayout01 = new QVBoxLayout(this);
    pLayout01->addWidget(mpView);
    //this->setLayout(pLayout01);

    pName = new QLineEdit();
    pCmd = new QLineEdit();
    pNote = new QLineEdit();
    QRegExp regExp("[a-fA-F0-9 ]{100}");
    pName->setValidator(new QRegExpValidator(regExp, this));
    pCmd->setValidator(new QRegExpValidator(regExp, this));
    pNote->setValidator(new QRegExpValidator(regExp, this));

    pLayout01->addWidget(pName);
    pLayout01->addWidget(pCmd);
    pLayout01->addWidget(pNote);

    QHBoxLayout *pLayout02 = new QHBoxLayout();
    QPushButton *pUpdateBtn = new QPushButton("更新");
    QPushButton *pCancelBtn = new QPushButton("取消");
    QPushButton *pOKBtn = new QPushButton("确定");

    connect(pOKBtn, &QPushButton::released, this, &DataBase::ok_btn_clicked);
    connect(pCancelBtn, &QPushButton::released, this, &DataBase::ok_btn_clicked);
    connect(pUpdateBtn, &QPushButton::released, this, &DataBase::update_btn_clicked);

    pLayout02->addStretch(1);
    pLayout02->addWidget(pOKBtn);
    pLayout02->addStretch(1);
    pLayout02->addWidget(pCancelBtn);
    pLayout02->addStretch(1);
    pLayout02->addWidget(pUpdateBtn);
    pLayout02->addStretch(1);

    pLayout01->addLayout(pLayout02);

    connect(mpView, &QTableView::doubleClicked, this, cmd_doubleclicked);

    this->setWindowTitle("设置");
    this->setWindowIcon(QIcon(":/new/prefix2/Image/setting.png"));
    this->resize(370, 360);
    this->setModal(true);
    this->setStyleSheet("QWidget{background-color: rgb(233, 233, 233);"
                        "font: 10.5pt}"
                        "QPushButton{background-color: rgb(199, 199, 199);"
                        "border-radius: 4px;"
                        "min-height: 29px; "
                        "min-width: 100px;  "
                        "font: 10.5pt;"
                        "/*border: 1px groove gray;*/}"
                        "QPushButton:hover {background-color: lime;}"
                        "QPushButton:pressed{background-color:rgb(255, 170, 0);"
                        "border-style: inset;}"
                        "QLineEdit{background-color: rgb(255, 255, 255)};");
}

void DataBase::cmd_doubleclicked(const QModelIndex &index)
{
//    qDebug()<<"double clicked,row = "<<index.row();
//    qDebug()<<"column = "<<index.column();

    pName->setText(getRecord(index.row()).value(0).toString());
    pCmd->setText(getRecord(index.row()).value(1).toString());
    pNote->setText(getRecord(index.row()).value(2).toString());
}
void DataBase::ok_btn_clicked()
{

}

void DataBase::update_btn_clicked()
{

    QString str;
//    str = "select * from MainTable where ID like '"+pName->text()+"%'";
//    mpModel->setQuery(str);

    str = "UPDATE MainTable SET note = '"+pNote->text()+"' WHERE ID = '"+pName->text()+"'";
    mpModel->setQuery(str);


    str = "UPDATE MainTable SET cmd = '"+pCmd->text()+"' WHERE ID = '"+pName->text()+"'";
    mpModel->setQuery(str);


    mpModel->setQuery("select * from MainTable");

    mpView->setColumnWidth(0,50);//设置主键所在列的宽度
    mpView->setColumnWidth(1,180);//设置主键所在列的宽度
    mpView->setColumnWidth(2,60);//设置主键所在列的宽度

}
void DataBase::cancel_btn_clicked()
{
    qDebug()<<"cancel_btn_clicked ";
}

/*
 *函数功能：创建数据库连接，初始化数据了视图，指定初始化的数据库表tableName
 *返回值： -1 创建数据库链接失败；
 *         1 正常执行。
 */
int DataBase::InitDataBase(void)
{
    int flag = createConnection();

    if(flag & (0x01))
    {
        qDebug()<<QObject::tr("数据库创建连接失败或者数据已经存在！");
    }
    if(flag & (0x02))
    {
        qDebug()<<QObject::tr("MainTable表创建失败或者该表已经存在！");
    }
    if(flag & (0x04))
    {
        qDebug()<<QObject::tr("Device表创建失败或者该表已经存在！");
    }

    return 1;
}
/*
 *函数功能：初始化DeviceView,设备信息视图
 */
void DataBase::InitDeviceView()
{
    //设备列表视图
    //mpModel = new QSqlQueryModel(this);
    //mpView->setModel(mpModel);

    mpView->setSelectionBehavior(QAbstractItemView::SelectRows);//每次选中一行
    mpView->horizontalHeader()->setStretchLastSection(true);//最后一行的拉伸
    mpView->setAlternatingRowColors(true);//行颜色交替显示

    mpModel->setQuery("select * from MainTable");

    mpModel->setHeaderData(0,Qt::Horizontal,tr("名称"));
    mpModel->setHeaderData(1,Qt::Horizontal,tr("指令"));
    mpModel->setHeaderData(2,Qt::Horizontal,tr("note"));

    mpView->setColumnWidth(0,50);//设置主键所在列的宽度
    mpView->setColumnWidth(1,180);//设置主键所在列的宽度
    mpView->setColumnWidth(2,60);//设置主键所在列的宽度

    mpView->show();
}
/*
 * 获取数据库中数据的条数
 *返回的条数是最近一次根据数据库进行查询后的结果集的条数
*/
int DataBase::GetTableCount()
{
    mpModel->query().last();
    return mpModel->query().at()+1;
}
/*
 *功能：将TableView导出到Excel文件
 *参数： fileName:带路径和Excel文件名的QString，否则无效
 *      TableName：要生成Excel的数据库表名称
 *      SheetName: 选择Excel的页名称
 *返回：-2：数据库连接创建失败
 *     -3：数据库打开失败
 *     -4：创建Excel页，及表格失败
 *      1：数据插入成功
 *注释：Excel每页可以存65535条数据。
 *参考：http://blog.163.com/sdzhangyuyin@126/blog/static/138413748201461594738437/
 */
int DataBase::ExportExcel(QString fileName,
                          QString TableName,
                          QString SheetName)
{
    {
        //首先创建页名，即sheet的名称
        QString sheetName;
        sheetName = SheetName;//"page";
        //"fileName"必须是一个带路径的文件名QString，否则无效
        qDebug() <<"fileName"<<fileName;
        //
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "connection");
        if(!db.isValid())
        {
            qDebug() << "export2Excel failed: QODBC not supported.";
            return -2;
        }
        //设置DSN
        QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").
                arg(fileName).arg(fileName);
        db.setDatabaseName(dsn);
        if(!db.open())
        {
            qDebug() << "export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
            QSqlDatabase::removeDatabase("connection");
            return -3;
        }

        //如果表已经存在的话，就删除表格
        QSqlQuery query(db);
        QString  sSql = QString("DROP TABLE [%1] ").arg(sheetName);
        query.exec(sSql);

        //创建表格,此表格作为Excel中的页，但是创建后会将原来的页删除
        sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
        sSql+="[ID] char(20),[Node] char(20),[Channel] char(20),[value] char(20),[time] char(20)";
        sSql += ")";
        qDebug()<<"sSql"<<sSql;
        query.prepare(sSql);//在调用query.exec()之后才能执行

        if(!query.exec())
        {
            qDebug() << "export2Excel failed: Create Excel sheet failed.";
            db.close();
            QSqlDatabase::removeDatabase("connection");
            return -4;
        }
        /*
     *插入数据，根据表获得导入到Excel中的数据
     *说明，此处用的是QSQLITE数据库，视图/文档模型
     *     Excel用的是ODBC驱动。首先根据SQL语句获得QSQLITE的结果集
     *     然后将model中的结果集插入到Excel数据库中
     */
        //第一步，根据表名称获得符合条件的结果集,将结果集显示到TableView中。
        mpModel->setQuery("select * from "+TableName);//结果集放在model中
        //第二步，获得表中结果集中数据的条数
        int mRecordCount;
        mpModel->query().last();
        mRecordCount = mpModel->query().at()+1;
        //设置进度条
        //mpProgressBar = new MProgressBar();
        //mpProgressBar->setValueRange(0,mRecordCount-1);
        //mpProgressBar->show();
        //第三步，将View中的数据依次导出到指定的Excel页中
        for(int i=0;i<mRecordCount;i++)
        {
            sSql =QString("INSERT INTO [%1] (").arg(sheetName);
            query.prepare( sSql+"[ID],[Node],[Channel],[value],[time])"
                                "VALUES ([:ID],[:Node],[:Channel],[:value],[:time])");
            query.bindValue(":ID", mpModel->record(i).value("ID").toString());
            query.bindValue(":Node", mpModel->record(i).value("Node").toString());
            query.bindValue(":Channel", mpModel->record(i).value("Channel").toString());
            query.bindValue(":value", mpModel->record(i).value("value").toString());
            query.bindValue(":time", mpModel->record(i).value("time").toString());
            query.exec();//必须存在，不然的话无法执行SQL语句

            //mpProgressBar->setBarValue(i);//进度条值设置
        }
        //mpProgressBar->close();//进度条窗口关闭
        //delete mpProgressBar;//进度条类内存回收
        query.clear();
        db.close();
    }
    //删除数据库连接
    QSqlDatabase::removeDatabase("connection");
    return 1;
}
/*
 *函数功能：创建数据库链接
 */
int DataBase::createConnection()
{
    int flag=0x00;
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE");
    db1.setDatabaseName("cmd_voice1.db");
    if(!db1.open())
    {
        QMessageBox::critical(0,"Cannot open database1",
                              "Unable to establish a database connection",
                              QMessageBox::Cancel);
        flag=0x01;
        return flag;
    }
    QSqlQuery query(db1);
    //创建总的数据记录表
    flag = query.exec(QString("create table MainTable(ID varchar primary key," /*1--cmd name，ID*/
                              "cmd varchar,"       /*2--指令*/
                              "note varchar)"));    /*3--note*/
    if(!flag)
    {
        flag = 0x02;
    }

    return flag;
}
/*
 *函数功能：按照命令name 进行查询
 *函数参数：time 格式:yyyy-MM-dd ，QString类型
 *返回数据：true，执行成功；
 *        false，执行失败。
 */
void DataBase::nameSearch(QString time)
{
    //bool flag;
    QString str;
    str = "select * from MainTable where time like '"+time+"%'";
    mpModel->setQuery(str);
}
/*
 *函数功能：数据库的查询，按照时间查询
 *函数参数：time 格式:yyyy-MM-dd ，QString类型
 *返回数据：true，执行成功；
 *        false，执行失败。
 */
void DataBase::timeSearch(QString time)
{
    //bool flag;
    QString str;
    str = "select * from MainTable where time like '"+time+"%'";
    mpModel->setQuery(str);
    //qDebug()<<str;
    //return flag;
}
/*
 *函数功能：按照节点和通道进行数据库查询
 *函数参数：节点，node;通道 channel
 *返回数据: 查询成功返回true,查询失败返回false;
 */
void DataBase::nodeSearch(QString node,
                          int channel)
{
    QString str;
    if(channel)
    {
        str = "select * from MainTable where Node ='"+
                node+"' and channel ="+QString::number(channel,10);
    }
    else
    {
        str = "select * from MainTable where Node ='"+node+"'";
    }
    qDebug()<<str;
    mpModel->setQuery(str);

    //return flag;
}
/*
 *函数功能：返回全部的数据
 */
void DataBase::ScreenDataBase()
{
    mpModel->setQuery("select * from MainTable");
}
/*
 *函数功能：按照时间，节点，通道来查询
 */
void DataBase::timeNodeSearch(QString time,
                              QString node,
                              int channel)
{
    QString str;
    if(channel)
    {
        str = "select * from MainTable where Node ='"+
                node+"' and channel ="+QString::number(channel,10);
    }
    else
    {
        str = "select * from MainTable where Node ='"+node+"'";
    }
    str = str+" and time like '"+time+"%'";
    str = "select * from MainTable";
    qDebug()<<str;
    mpModel->setQuery(str);
}

/*
 *函数功能：获取QRecord
 */
QSqlRecord DataBase::getRecord(int row)
{
    mpModel->query().last();
    int num = mpModel->query().at();
    if(row <= num)
        return mpModel->record(row);
    else
        return QSqlRecord();
}
/*
 *函数功能：获取数据库中record的数目
 */
long DataBase::getRecordNum(QString tablename)
{
    if(tablename == "MainTable")
        return mpModel->rowCount();//获取表格中的数据条数
    if(tablename == "DeviceTable")
        return mpDeviceModel->rowCount();
    else
        return 0;
}
/*
 *函数功能：往数据库MainTable表中插入一条记录
 */
bool DataBase::insertRecord(
        //int keyId,
        QString name,
        QString cmd,
        QString note)
{
    QString KeyID;//QString::number(getRecordNum()+1,10);
    QString Node = cmd;

    KeyID = name;//.toString("yyyyMMddhhmmss");
    mpModel->setQuery(QString("insert into MainTable values('"+
                              KeyID+"','"+//ID
                              Node+"','"+       //节点,QString型，需要在SQL语句中添加''
                              note+"')"));//时间

    mpModel->setQuery("select * from MainTable");

    mpView->setColumnWidth(0,50);//设置主键所在列的宽度
    mpView->setColumnWidth(1,180);//设置主键所在列的宽度
    mpView->setColumnWidth(2,60);//设置主键所在列的宽度
    mpView->verticalScrollBar()
            ->setSliderPosition(mpModel->rowCount());//QTableView的定位，定位到表格底部
    //考虑效率问题，这是QSqlQueryModel的默认只取255条的限制。
    while(mpModel->canFetchMore())
        mpModel->fetchMore();//不加此句的话滚动条将值滚动到256的位置
    //将tableview的滚动条设置到底部
    mpView->scrollToBottom();
    mpView->show();//将结果集中的内容显示到视图中

    return true;
}
/*
 *函数功能：往数据库DeviceTable表中插入一条设备记录
 *        如果该deviceId的设备已经存在，则现将其删除然后再插入
 *        重载函数。
 */
void DataBase::insertDeviceRecord(QTableView *mpDeviceView,
                                  QString deviceId,//该ID已经是节点Id和通道Id
                                  QString nodename,
                                  QString nodearea,
                                  QString alertTvalue,
                                  QString sampleinterval,
                                  QString channelname)
{
    QString alertTvalue1 = alertTvalue+QObject::tr("℃");
    if("" == alertTvalue)
        alertTvalue1 = "";
    QString sampleinterval1 = sampleinterval+"s";//采样间隔
    if(sampleinterval == "")
        sampleinterval1 = "";

    mpDeviceModel->setQuery(QString("delete from DeviceTable where DeviceId=")+
                            deviceId);

    mpDeviceModel->setQuery(QString("insert into DeviceTable values('"+
                                    deviceId+"','"+/*1，设备ID*/
                                    nodename+"','"+/*2，设备名称*/
                                    nodearea+"','"+/*3，设备所在区域*/
                                    alertTvalue1+"','"+/*4，温度阀值*/
                                    sampleinterval1+"','"+/*5，采样间隔*/
                                    channelname+"')"));/*6，通道名称*/
    mpDeviceModel->setQuery("select * from DeviceTable");
}
/*
 *函数功能：往数据库DeviceTable表中插入一条设备记录
 *        如果该deviceId的设备已经存在，则现将其删除然后再插入。
 *        重载函数。
 *@参数：数据库中的一条完整记录，QSqlRecord。
 */
void DataBase::insertDeviceRecord(QSqlRecord mRecord)
{
    //温度阀值
    QString alertTvalue1;
    if(mRecord.value(3).toString().right(1) == QObject::tr("℃"))
        alertTvalue1 = mRecord.value(3).toString();
    else
        alertTvalue1 = mRecord.value(3).toString()
                +QObject::tr("℃");
    if(QObject::tr("℃") == alertTvalue1)
        alertTvalue1 = "";
    //采样间隔
    QString sampleinterval1;
    if(mRecord.value(4).toString().right(1) == "s")
        sampleinterval1 = mRecord.value(4).toString();
    else
        sampleinterval1 = mRecord.value(4).toString()+"s";//采样间隔
    if(sampleinterval1 == "s")
        sampleinterval1 = "";

    //将原来的记录删除
    mpDeviceModel->setQuery(QString("delete from DeviceTable where DeviceId='")+
                            mRecord.value(0).toString()+"'");
    //插入新的记录
    mpDeviceModel->setQuery(QString("insert into DeviceTable values('"+
                                    mRecord.value(0).toString()+"','"+/*1，设备ID*/
                                    mRecord.value(1).toString()+"','"+/*2，设备名称*/
                                    mRecord.value(2).toString()+"','"+/*3，设备所在区域*/
                                    alertTvalue1+"','"+/*4，温度阀值*/
                                    sampleinterval1+"','"+/*5，采样间隔*/
                                    mRecord.value(5).toString()+"')"));/*6，通道名称*/
    mpDeviceModel->setQuery("select * from DeviceTable");
}
/*
 *函数功能：从设备数据表中获取指定id的一条记录。
 *函数返回：若存在指定设备Id的记录，则返回第一条记录，
 *         若不存在则返回一条空记录。
 *注   意：使用此函数时，一定要记得判断返回的记录是否为指定的ID，
 *        否则是不存在此设备Id的设备记录。
 */
QSqlRecord DataBase::getDeviceRecord(QString deviceId)
{
    QString str = QString("select * from DeviceTable where DeviceId='")+
            deviceId+"'";
    mpDeviceModel->setQuery(str);
    //获取指定deviceId的记录
    QSqlRecord r = mpDeviceModel->record(0);
    qDebug()<<str;
    mpDeviceModel->
            setQuery(QString("select * from DeviceTable"));
    return r;
}

/*
 *查询设备列表，并将结果集显示到指定的view中
 */
void DataBase::updateDeviceView(QString deviceId)//QTableView *mpDeviceView
{
    //mpDeviceView = mpDeviceView;
    //    long l = getRecordNum("DeviceTable");
    //    for(long i=0;i<l;i++)
    //    {
    //        qDebug()<<mpDeviceModel->record(i).value(0).toString();
    //    }

    mpDeviceModel->setQuery(
                QString("select * from DeviceTable where DeviceId='131a008'"));
    //获取指定deviceId的记录
    QSqlRecord r = mpDeviceModel->record(0);
    qDebug()<<r;
}


