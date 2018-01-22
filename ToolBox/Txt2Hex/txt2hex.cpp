#include "txt2hex.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

Txt2Hex::Txt2Hex(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLoadBtn  = new QPushButton("Load");
    pLoadPath = new QLineEdit();
    pLayout01->addWidget(pLoadBtn);
    pLayout01->addWidget(pLoadPath);

    QHBoxLayout *pLayout02 = new QHBoxLayout();
    pSaveBtn  = new QPushButton("Save");
    pSavePath = new QLineEdit();
    pLayout02->addWidget(pSaveBtn);
    pLayout02->addWidget(pSavePath);

    QVBoxLayout *pLayout03 = new QVBoxLayout();
    pLayout03->addLayout(pLayout01);
    pLayout03->addLayout(pLayout02);

    pConvert = new QPushButton("Convert");
    pConvert->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

    QHBoxLayout *pLayout04 = new QHBoxLayout();
    pLayout04->addLayout(pLayout03);
    pLayout04->addWidget(pConvert);

    QLabel *pTxtLabel = new QLabel("Text Edit:");
    pTxtEdit = new QPlainTextEdit();
    QLabel *pHexLabel = new QLabel("Hex Edit:");
    pHexEdit = new QPlainTextEdit();

    QVBoxLayout *pLayout05 = new QVBoxLayout(this);
    pLayout05->addLayout(pLayout04);
    pLayout05->addWidget(pTxtLabel);
    pLayout05->addWidget(pTxtEdit);
    pLayout05->addWidget(pHexLabel);
    pLayout05->addWidget(pHexEdit);


    /*消息设置*/
    connect(pLoadBtn, &QPushButton::released, this, &Txt2Hex::on_load_btn_released);
    connect(pSaveBtn, &QPushButton::released, this, &Txt2Hex::on_save_btn_released);
    connect(pConvert, &QPushButton::released, this, &Txt2Hex::on_convert_btn_released);
}

void Txt2Hex::on_load_btn_released()
{
    qDebug()<<"load btn release.";
    QString filestr = QFileDialog::getOpenFileName(this, tr("选取文件"), "","*.txt",0);
    if (filestr.isNull())
        return;
    pLoadPath->setText(filestr);
}

void Txt2Hex::on_save_btn_released()
{
    qDebug()<<"save btn release.";
    QString fileout = QFileDialog::getOpenFileName(this, tr("输出文件"), "","*.bin",0);
    if (!fileout.isNull())
        pSavePath->setText(fileout);
}

void Txt2Hex::on_convert_btn_released()
{
    qDebug()<<"convert btn release.";
    /*打开原始文件*/
    QFile loadfile(pLoadPath->text());
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
    pTxtEdit->setPlainText(str);
    if (arr.isNull())
    {
        qDebug()<<"no string: " + QString(LABEL_STR);

        return;
    }

    /*设置转换后的文件*/
    QFile filesave(pSavePath->text());
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

    pHexEdit->setPlainText(Hex2Text_handle(arr));
}
/*HEX转文本*/
QString Txt2Hex::Hex2Text_handle(QByteArray input)
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
QByteArray Txt2Hex::text2Hex_handle(QString input)
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
