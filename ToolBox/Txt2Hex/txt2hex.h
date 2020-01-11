#ifndef TXT2HEX_H
#define TXT2HEX_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextStream>

class Txt2Hex : public QWidget
{
    Q_OBJECT
public:
    explicit Txt2Hex(QWidget *parent = nullptr);

    QPushButton *pLoadBtn;
    QPushButton *pSaveBtn;
    QLineEdit   *pLoadPath;
    QLineEdit   *pSavePath;
    QPushButton *pConvert;
    QPlainTextEdit *pTxtEdit;
    QPlainTextEdit *pHexEdit;

    QByteArray text2Hex_handle(QString input);
    QString Hex2Text_handle(QByteArray input);

signals:

public slots:
    void on_load_btn_released();
    void on_save_btn_released();
    void on_convert_btn_released();
};

#endif // TXT2HEX_H
