#ifndef HEX2TXT_H
#define HEX2TXT_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QByteArray>

class Hex2Txt : public QWidget
{
    Q_OBJECT
public:
    explicit Hex2Txt(QWidget *parent = nullptr);

    QPushButton *pLoadBtn ;
    QLineEdit   *pLoadPath;
    QPushButton *pSaveBtn ;
    QLineEdit   *pSavePath;
    QPushButton *pConvert;
    QPlainTextEdit *pHexEdit ;
    QPlainTextEdit *pTxtEdit ;

    QString Hex2Text_handle(QByteArray input);

public slots:
    void on_load_btn_released();
    void on_save_btn_released();
    void on_convert_btn_released();


};

#endif // HEX2TXT_H
