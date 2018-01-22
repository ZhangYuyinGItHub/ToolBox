#include "hex2txt.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

Hex2Txt::Hex2Txt(QWidget *parent) : QWidget(parent)
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

    QLabel *pHexLabel        = new QLabel("Hex Edit:");
    pHexEdit = new QPlainTextEdit();
    QLabel *pTxtLabel        = new QLabel("Text Edit:");
    pTxtEdit = new QPlainTextEdit();
    QVBoxLayout *pLayout05   = new QVBoxLayout(this);
    pLayout05->addLayout(pLayout04);
    pLayout05->addWidget(pHexLabel);
    pLayout05->addWidget(pHexEdit);
    pLayout05->addWidget(pTxtLabel);
    pLayout05->addWidget(pTxtEdit);

    /*消息设置*/
    connect(pLoadBtn, &QPushButton::released, this, &Hex2Txt::on_load_btn_released);
    connect(pSaveBtn, &QPushButton::released, this, &Hex2Txt::on_save_btn_released);
    connect(pConvert, &QPushButton::released, this, &Hex2Txt::on_convert_btn_released);
}

void Hex2Txt::on_load_btn_released()
{
    qDebug()<<"load btn release.";
    QString filestr = QFileDialog::getOpenFileName(this, tr("选取二进制文件"), "","*.bin ;; *.dat",0);
    if (filestr.isNull())
        return;
    pLoadPath->setText(filestr);
}

void Hex2Txt::on_save_btn_released()
{
    qDebug()<<"save btn release.";
    QString fileout = QFileDialog::getOpenFileName(this, tr("输出文本文件"), "","*.txt;;*.*",0);
    if (!fileout.isNull())
        pSavePath->setText(fileout);
}
/*HEX转文本*/
QString Hex2Txt::Hex2Text_handle(QByteArray input)
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
void Hex2Txt::on_convert_btn_released()
{
    /*打开原始文件*/
    QFile loadfile2(pLoadPath->text());
    if(!loadfile2.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray arr;
    arr = loadfile2.readAll();

    QString str = Hex2Text_handle(arr);
    pHexEdit->setPlainText(str);
    //qDebug()<<arr;
    loadfile2.close();

    /*打开保存文件*/
    QFile savefile2(pSavePath->text());
    if(!savefile2.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream qts(&savefile2);
    qts<<str;
    pTxtEdit->setPlainText(str);
    savefile2.close();
    QMessageBox::information(NULL, "Tip", tr("success! "),
                             QMessageBox::Ok );
}
