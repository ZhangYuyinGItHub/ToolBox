#ifndef HEX2TXT_H
#define HEX2TXT_H

#include <QWidget>

class Hex2Txt : public QWidget
{
    Q_OBJECT
public:
    explicit Hex2Txt(QWidget *parent = nullptr);

public slots:
    void on_load_btn_released();
    void on_save_btn_released();
    void on_convert_btn_released();
};

#endif // HEX2TXT_H
