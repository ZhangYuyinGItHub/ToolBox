#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QDataStream>
#include <QByteArrayData>
#include <QDebug>
#include "./Hex2Txt/hex2txt.h"
#include "./Txt2Hex/txt2hex.h"
#include "./SerialAssit/serialassit.h"
#include "./about/about.h"
#include "./voice/sbc.h"

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();

    //pPacket->show();
    Hex2Txt* pHex2Tex = new Hex2Txt();
    pTabWidget->insertTab(0, pHex2Tex, "HexToTxt");

    Txt2Hex *pTxt2Hex = new Txt2Hex();
    pTabWidget->insertTab(1, pTxt2Hex, "Txt2Hex");

    Packet * pPacket = new Packet();
    pTabWidget->insertTab(2, pPacket, "Packet");

    SerialAssit *pSerialAssit = new SerialAssit();
    pTabWidget->insertTab(3, pSerialAssit, "SerialAssit");

    sbc *psbc = new sbc();
    pTabWidget->insertTab(4, psbc, "Voice");

    about *pAbout = new about();
    pTabWidget->insertTab(5, pAbout, "About");


    this->setContentsMargins(10,0,10,10);
    this->setCentralWidget(pTabWidget);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
