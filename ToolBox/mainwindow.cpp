#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QDataStream>
#include <QByteArrayData>
//#include <QDebug>
#include "./Hex2Txt/hex2txt.h"
#include "./Txt2Hex/txt2hex.h"
#include "./SerialAssit/serialassit.h"
#include "./about/about.h"
#include "./voice/voice.h"
#include "./AES/aesdecrypt.h"

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setStyleSheet("background-color: rgb(107, 147, 255)");
//    this->setStyleSheet("QPushButton{background-color: rgb(107, 147, 255);"
//                        "board-radius: 2px;}"
//                        "QPushButton:hover {background-color: lime;}");

    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setStyleSheet("QWidget{background-color: rgb(233, 233, 233);"
                              "font: 10.5pt}"
                              "QPushButton{background-color: rgb(199, 199, 199);"
                              "border-radius: 3px;"
                              "min-height: 23px; "
                              "min-width: 100px;  "
                              "font: 10.5pt;"
                              "/*border: 1px groove gray;*/}"
                              "QPushButton:hover {background-color: lime;}"
                              "QPushButton:pressed{background-color:rgb(255, 170, 0);"
                                                  "border-style: inset;}"
                              "QLineEdit{background-color: rgb(255, 255, 255)};");


    //pPacket->show();
    Hex2Txt* pHex2Tex = new Hex2Txt();
    pTabWidget->insertTab(0, pHex2Tex, "Hex2Txt");

    Txt2Hex *pTxt2Hex = new Txt2Hex();
    pTabWidget->insertTab(1, pTxt2Hex, "Txt2Hex");

//    Packet * pPacket = new Packet();
//    pTabWidget->insertTab(2, pPacket, "Packet");

    SerialAssit *pSerialAssit = new SerialAssit();
    pTabWidget->insertTab(3, pSerialAssit, "SerialAssit");

    voice *psbc = new voice();
    pTabWidget->insertTab(4, psbc, "Voice");

    about *pAbout = new about();
    pTabWidget->insertTab(5, pAbout, "About");

    AesDecrypt *pAesDecrypt = new AesDecrypt();
    pTabWidget->insertTab(2, pAesDecrypt, "Aes");


    this->setContentsMargins(10,0,10,10);
    this->setCentralWidget(pTabWidget);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
