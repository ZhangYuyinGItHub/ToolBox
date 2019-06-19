#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QIODevice>

class audiodevice : public QIODevice
{

private:
    QByteArray data_pcm; //存放pcm数据
    int        len_written; //记录已写入多少字节

public:
    audiodevice(QByteArray pcm); //创建对象传递pcm数据
    ~audiodevice();

    qint64 readData(char *data, qint64 maxlen); //重新实现的虚函数
    qint64 writeData(const char *data, qint64 len); //它是个纯虚函数， 不得不实现


};

#endif // AUDIODEVICE_H
