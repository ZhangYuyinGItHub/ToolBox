#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QDataStream>
#include <QByteArrayData>
#include <QDebug>


#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

QByteArray text2Hex_handle(QString input);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pPacket = new Packet(ui->Packet);

    //mPacket.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadBtn_clicked()
{
    QString filestr = QFileDialog::getOpenFileName(this, tr("选取文件"), "","*.txt",0);
    if (filestr.isNull())
        return;
    ui->loadPathEdit->setText(filestr);


    //pPacket->setVisible(true);
}
/*HEX转文本*/
QString Hex2Text_handle(QByteArray input)
{
    QString str = "";
    int len = input.length();
    //return str = input.toHex(20);//中间以空格隔开
    for (int index = 0; index < len; index++)
    {
        QByteArray b;
        b[0] = input[index];
        str += LABEL_STR;
        str += b.toHex();
    }
    return str;
}
/*文本转二进制*/
QByteArray text2Hex_handle(QString input)
{
    if (input.isNull())
        return NULL;
    QByteArray arr;
    int len = input.length();
    for(int offset = 0,i = 0; input.indexOf(LABEL_STR, offset)>=0;)
    {
        offset = input.indexOf(LABEL_STR, offset);
        if (offset <= (len - LABEL_VALUE_LEN - 1))
        {
            offset += LABEL_LEN;
        }
        else
        {
            break;
        }
        bool ok;
        QString str_value = input.mid(offset, LABEL_VALUE_LEN);
        /*判断后面的两个数据是否都是有效的数字*/
        if (QString::compare(str_value.mid(0, 1),"0") < 0)
            continue;
        if ((QString::compare(str_value.mid(0, 1),"9") > 0) && (QString::compare(str_value.mid(0, 1),"A"))<0)
            continue;
        if ((QString::compare(str_value.mid(0, 1),"F") > 0) && (QString::compare(str_value.mid(0, 1),"a"))<0)
            continue;
        if ((QString::compare(str_value.mid(0, 1),"f") > 0))
            continue;

        if (QString::compare(str_value.mid(1, 1),"0") < 0)
            continue;
        if ((QString::compare(str_value.mid(1, 1),"9") > 0) && (QString::compare(str_value.mid(1, 1),"A"))<0)
            continue;
        if ((QString::compare(str_value.mid(1, 1),"F") > 0) && (QString::compare(str_value.mid(1, 1),"a"))<0)
            continue;
        if ((QString::compare(str_value.mid(1, 1),"f") > 0))
            continue;

        char c = QString(str_value).toInt(&ok, 16);
        arr[i++]  = c;
    }
    return arr;
}


void MainWindow::on_saveBtn_clicked()
{
    QString fileout = QFileDialog::getOpenFileName(this, tr("输出文件"), "","*.bin",0);
    if (!fileout.isNull())
        ui->savePathEdit->setText(fileout);
}

void MainWindow::on_convertBtn_clicked()
{
    /*打开原始文件*/
    QFile loadfile(ui->loadPathEdit->text());
    if(!loadfile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //cout<<"open file failed!";
        return;
    }
    QTextStream qs(&loadfile);
    //QString str = qs.readAll();
    // ui->loadFileText->setPlainText(str);

    /*数据转换*/
    QByteArray arr ;
    QString str  = "";
    for (;!qs.atEnd();)
    {
        QString temp;
        temp = qs.readLine();
        arr += text2Hex_handle(" "+temp);
        str += temp + "\n";
    }
    ui->loadFileText->setPlainText(str);
    if (arr.isNull())
    {
        qDebug()<<"no string: " + QString(LABEL_STR);

        return;
    }

    /*设置转换后的文件*/
    QFile filesave(ui->savePathEdit->text());
    if(!filesave.open(QIODevice::WriteOnly))
    {
        return;
    }
    /*以二进制的方式写入文件中*/
    filesave.write(arr);

    /**该方式第一个字节会自动按照int进行补全，目前无法解决*/
    {
        //QDataStream qw(&fw);
        //qw.setVersion(QDataStream::Qt_5_7);
        //qw<<arr;
    }

    QMessageBox::information(NULL, "Tip", tr("success! "),
                             QMessageBox::Ok );
    loadfile.close();
    filesave.close();

    ui->saveFileText->setPlainText(Hex2Text_handle(arr));
}

/**************************************************
 *
 * <Hex to Text> tab page
 *
***************************************************/
/*============= Tab Page2 ==================*/
void MainWindow::on_loadBtn_2_clicked()
{
    QString filestr = QFileDialog::getOpenFileName(this, tr("选取二进制文件"), "","*.bin ;; *.dat",0);
    if (filestr.isNull())
        return;
    ui->loadPathEdit_2->setText(filestr);
}

void MainWindow::on_saveBtn_2_clicked()
{
    QString fileout = QFileDialog::getOpenFileName(this, tr("输出文本文件"), "","*.txt;;*.*",0);
    if (!fileout.isNull())
        ui->savePathEdit_2->setText(fileout);
}

void MainWindow::on_convertBtn_2_clicked()
{
    /*打开原始文件*/
    QFile loadfile2(ui->loadPathEdit_2->text());
    if(!loadfile2.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray arr;
    arr = loadfile2.readAll();

    QString str = Hex2Text_handle(arr);
    ui->loadFileText_2->setPlainText(str);
    //qDebug()<<arr;
    loadfile2.close();

    /*打开保存文件*/
    QFile savefile2(ui->savePathEdit_2->text());
    if(!savefile2.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream qts(&savefile2);
    qts<<str;
    ui->saveFileText_2->setPlainText(str);
    savefile2.close();
}
