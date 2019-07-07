#include "zcombox.h"

zComBox::zComBox(QWidget *parent):QComboBox(parent)
{

}
zComBox::~zComBox()
{

}

void zComBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit combox_clicked();  //触发clicked信号
    }

    QComboBox::mousePressEvent(event);  //将该事件传给父类处理，这句话很重要，如果没有，父类无法处理本来的点击事件
}
