#ifndef AESDECRYPT_H
#define AESDECRYPT_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>

class AesDecrypt : public QWidget
{
    Q_OBJECT
public:
    explicit AesDecrypt(QWidget *parent = 0);


public:
    QLineEdit * pSrcPathLineEdit;
    QLineEdit * pDesPathLineEdit;

    QPushButton *pLoadBtn;
    QPushButton *pSaveBtn;

    QPushButton *pEncrypt;
    QPushButton *pDecrypt;

    QPlainTextEdit *pEncryptEdit ;
    QPlainTextEdit *pDecryptEdit ;

    QString Hex2Text_handle(QByteArray input);

signals:


public slots:
    void on_load_btn_released();
    void on_save_btn_released();
    void on_Decrypt_btn_released();
    void on_encrypt_btn_released();
};

#endif // AESDECRYPT_H
