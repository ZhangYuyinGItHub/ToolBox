#include "txt2hex.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDebug>

Txt2Hex::Txt2Hex(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *pLayout01 = new QHBoxLayout();
    QPushButton *pLoadBtn  = new QPushButton("Load");
    QLineEdit   *pLoadPath = new QLineEdit();
    pLayout01->addWidget(pLoadBtn);
    pLayout01->addWidget(pLoadPath);

    QHBoxLayout *pLayout02 = new QHBoxLayout();
    QPushButton *pSaveBtn  = new QPushButton("Save");
    QLineEdit   *pSavePath = new QLineEdit();
    pLayout02->addWidget(pSaveBtn);
    pLayout02->addWidget(pSavePath);

    QVBoxLayout *pLayout03 = new QVBoxLayout();
    pLayout03->addLayout(pLayout01);
    pLayout03->addLayout(pLayout02);

    QPushButton *pConvert = new QPushButton("Convert");
    pConvert->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

    QHBoxLayout *pLayout04 = new QHBoxLayout();
    pLayout04->addLayout(pLayout03);
    pLayout04->addWidget(pConvert);

    QLabel *pTxtLabel = new QLabel("Text Edit:");
    QPlainTextEdit *pTxtEdit = new QPlainTextEdit();
    QLabel *pHexLabel = new QLabel("Hex Edit:");
    QPlainTextEdit *pHexEdit = new QPlainTextEdit();

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
}

void Txt2Hex::on_save_btn_released()
{
    qDebug()<<"save btn release.";
}

void Txt2Hex::on_convert_btn_released()
{
    qDebug()<<"convert btn release.";
}
