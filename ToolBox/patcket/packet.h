#ifndef PACKET_H
#define PACKET_H

#include <QWidget>
#include <QTabWidget>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QStringListModel>
#include <QMessageBox>

class Packet : public QWidget
{
    Q_OBJECT
public:
    explicit Packet(QWidget *parent = nullptr);

    //QVBoxLayout *pVLayout01;
    QListView         *pPacketFileListView;
    QPushButton       *pPacketLoadBtn;
    QPushButton       *pPacketBtn;
    QLabel            *pPacketFileInfoText;
    QGraphicsView     *pPacketMemView;
    QGraphicsScene    *pPacketScene;

    QStringList       *pPacketFileList;
    QStringListModel  *pPacketListModel;
    QLabel            *pPacketStatus;

    QByteArray HeadArr;
    QByteArray HexArr;

public:


protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void on_packet_btn_clicked();
    void on_files_load_btn_clicked();
    void on_file_list_pressed(QModelIndex index);
};

#endif // PACKET_H
