#ifndef TXT2HEX_H
#define TXT2HEX_H

#include <QWidget>

class Txt2Hex : public QWidget
{
    Q_OBJECT
public:
    explicit Txt2Hex(QWidget *parent = nullptr);

signals:

public slots:
    void on_load_btn_released();
    void on_save_btn_released();
    void on_convert_btn_released();
};

#endif // TXT2HEX_H
