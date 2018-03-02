#ifndef SERIALASSIT_H
#define SERIALASSIT_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class SerialAssit : public QWidget
{
    Q_OBJECT
public:
    explicit SerialAssit(QWidget *parent = nullptr);

    QLabel *pComNumLabel;
    QComboBox *pComNum;
    QLabel *pComBaudRateLabel;
    QComboBox *pComBaudRate;
    QPushButton *pComRefreshBtn;

protected:
    //void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void comfresh(void);
};

#endif // SERIALASSIT_H
