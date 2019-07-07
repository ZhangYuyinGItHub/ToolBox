#ifndef ZCOMBOX_H
#define ZCOMBOX_H

#include <QWidget>
#include <QComboBox>

#include <QMouseEvent>

class zComBox : public QComboBox
{
     Q_OBJECT    //只有加入Q_OBJECT，才能使用信号signal和槽slot的机制
public:
    explicit zComBox(QWidget *parent = 0);
    ~zComBox();

protected:
    virtual void mousePressEvent(QMouseEvent *e);  //添加鼠标点击事件

signals:
    void combox_clicked();  //自定义点击信号，在mousePressEvent事件发生时触发
};

#endif // ZCOMBOX_H
