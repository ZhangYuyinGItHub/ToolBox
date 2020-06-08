#include "aesdecrypt.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QLabel"
#include "QFileDialog"
#include "QTextStream"
#include <QMessageBox>

#include "aes/aes_hanlde_vocie.h"

#define  LABEL_STR         " "//"0x"
#define  LABEL_LEN         0x01
#define  LABEL_VALUE_LEN   0x02

AesDecrypt::AesDecrypt(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLoadBtn  = new QPushButton("Load");
    pSrcPathLineEdit = new QLineEdit();
    pSrcPathLineEdit->setPlaceholderText("input source file path...");
    pLayout01->addWidget(pLoadBtn);
    pLayout01->addWidget(pSrcPathLineEdit);

    QHBoxLayout *pLayout02 = new QHBoxLayout();
    pSaveBtn  = new QPushButton("Save");
    pDesPathLineEdit = new QLineEdit();
    pDesPathLineEdit->setPlaceholderText("input destination file path...");
    pLayout02->addWidget(pSaveBtn);
    pLayout02->addWidget(pDesPathLineEdit);

    QVBoxLayout *pLayout03 = new QVBoxLayout();
    pLayout03->addLayout(pLayout01);
    pLayout03->addLayout(pLayout02);

    pDecrypt = new QPushButton("DeCrypt");
    pDecrypt->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
    pEncrypt = new QPushButton("EnCrypt");
    pEncrypt->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);


    QHBoxLayout *pLayout04 = new QHBoxLayout();
    pLayout04->addLayout(pLayout03);
    pLayout04->addWidget(pEncrypt);
    pLayout04->addWidget(pDecrypt);

    QLabel *pHexLabel        = new QLabel("Input File:");
    pEncryptEdit = new QPlainTextEdit();
    QLabel *pTxtLabel        = new QLabel("Output File:");
    pDecryptEdit = new QPlainTextEdit();
    QVBoxLayout *pLayout05   = new QVBoxLayout(this);
    pLayout05->addLayout(pLayout04);
    pLayout05->addWidget(pHexLabel);
    pLayout05->addWidget(pEncryptEdit);
    pLayout05->addWidget(pTxtLabel);
    pLayout05->addWidget(pDecryptEdit);

    /*消息设置*/
    connect(pLoadBtn, &QPushButton::released, this, &AesDecrypt::on_load_btn_released);
    connect(pSaveBtn, &QPushButton::released, this, &AesDecrypt::on_save_btn_released);
    connect(pDecrypt, &QPushButton::released, this, &AesDecrypt::on_Decrypt_btn_released);
    connect(pEncrypt, &QPushButton::released, this, &AesDecrypt::on_encrypt_btn_released);
}


void AesDecrypt::on_load_btn_released()
{

    //qDebug()<<"load btn release.";
    QString filestr = QFileDialog::getOpenFileName(this, tr("选取二进制文件"), "","*.bin ;; *.dat",0);
    if (filestr.isNull())
        return;
    pSrcPathLineEdit->setText(filestr);

    QFile loadfile2(pSrcPathLineEdit->text());
    if(!loadfile2.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray arr;
    arr = loadfile2.readAll();

    QString str = Hex2Text_handle(arr);
    pEncryptEdit->setPlainText(str);
    ////qDebug()<<arr;
    loadfile2.close();
}

void AesDecrypt::on_save_btn_released()
{
    //qDebug()<<"save btn release.";
    QString fileout = QFileDialog::getOpenFileName(this, tr("输出文本文件"), "","*.txt;;*.*",0);
    if (!fileout.isNull())
        pDesPathLineEdit->setText(fileout);
}
/*HEX转文本*/
QString AesDecrypt::Hex2Text_handle(QByteArray input)
{
    QString str = "";
    int len = input.length();
    //return str = input.toHex(20);//中间以空格隔开
    for (int index = 0; index < len; index++)
    {
        QByteArray b;
        b[0] = input[index];

        str += b.toHex();
        str += LABEL_STR;
    }
    return str;
}

void AesDecrypt::on_encrypt_btn_released()
{
    /*1. get src file name, QString to char * */
    QByteArray ba;
    QString  lSrcFilePath = pSrcPathLineEdit->text();
    char*  pFileIn;
    ba = lSrcFilePath.toLatin1();
    pFileIn = ba.data();

    /*2. get des file name, QString to char*   */
    QString  lDesFilePath = pDesPathLineEdit->text();
    char*  pFileout;
    ba = lDesFilePath.toLatin1();
    pFileout = ba.data();

    /*4. decryption*/
    aes_data_encrypt(pFileIn, pFileout);


    /*5. display decrypted files in plaintext edit*/
    QFile savefile2(pDesPathLineEdit->text());
    if(!savefile2.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray arr;
    arr = savefile2.readAll();
    savefile2.close();

    QString des_str = Hex2Text_handle(arr);
    pDecryptEdit->setPlainText(des_str);
    QMessageBox::information(NULL, "Tip", tr("success! "),
                             QMessageBox::Ok );
}

void AesDecrypt::on_Decrypt_btn_released()
{
    /*1. get src file name, QString to char * */
    QByteArray ba;
    QString  lSrcFilePath = pSrcPathLineEdit->text();
    char*  pFileIn;
    ba = lSrcFilePath.toLatin1();
    pFileIn = ba.data();

    /*2. get des file name, QString to char*   */
    QString  lDesFilePath = pDesPathLineEdit->text();
    char*  pFileout;
    ba = lDesFilePath.toLatin1();
    pFileout = ba.data();

    /*4. decryption*/
    aes_data_decrypt(pFileIn, pFileout);


    /*5. display decrypted files in plaintext edit*/
    QFile savefile2(pDesPathLineEdit->text());
    if(!savefile2.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray arr;
    arr = savefile2.readAll();
    savefile2.close();

    QString des_str = Hex2Text_handle(arr);
    pDecryptEdit->setPlainText(des_str);
    QMessageBox::information(NULL, "Tip", tr("success! "),
                             QMessageBox::Ok );

}
