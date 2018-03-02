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
#include "./Hex2Txt/hex2txt.h"
#include "./Txt2Hex/txt2hex.h"
#include "./SerialAssit/serialassit.h"

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

QByteArray text2Hex_handle(QString input);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //pPacket->show();
    Hex2Txt* pHex2Tex = new Hex2Txt();
    ui->tabWidget->insertTab(0, pHex2Tex, "HexToTxt");

    Txt2Hex *pTxt2Hex = new Txt2Hex();
    ui->tabWidget->insertTab(1, pTxt2Hex, "Txt2Hex");

    Packet * pPacket = new Packet();
    ui->tabWidget->insertTab(2, pPacket, "Packet");

    SerialAssit *pSerialAssit = new SerialAssit();
    ui->tabWidget->insertTab(3, pSerialAssit, "SerialAssit");
}

MainWindow::~MainWindow()
{
    delete ui;
}
