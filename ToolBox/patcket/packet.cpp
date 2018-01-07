#include "packet.h"
#include <QApplication>


Packet::Packet(QWidget *parent) : QWidget(parent)
{
    if (parent == nullptr)
        parent = this;

    pPacketFileInfoText = new QLabel("File Info");
    pPacketLoadBtn      = new QPushButton("Load Files");
    pPacketBtn          = new QPushButton("Packet");
    pPacketFileListView = new QListView();
    pPacketMemView      = new QGraphicsView();
    pPacketScene        = new QGraphicsScene();
    pPacketFileList     = new QStringList();
    pPacketListModel    = new QStringListModel();
    pPacketStatus       = new QLabel();

    /*文件显示*/
    pPacketFileListView->setModelColumn(2);
    pPacketFileListView->setModel(pPacketListModel);
    //设置不可编辑状态
    pPacketFileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /*添加事件*/
    connect(pPacketLoadBtn,&QPushButton::pressed,
            this, &Packet::on_files_load_btn_clicked);
    connect(pPacketBtn,&QPushButton::pressed,
            this, &Packet::on_packet_btn_clicked);

    /*设置图像展示的颜色*/
    pPacketMemView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    pPacketMemView->setScene(pPacketScene);
    pPacketMemView->show();
    QBrush mBrush;
    mBrush.setStyle(Qt::Dense4Pattern);
    pPacketScene->setBackgroundBrush(mBrush);


    for (int i =0; i<30; i++)
    {
        QGraphicsRectItem *mRectItem = new QGraphicsRectItem(0,0,100,10);

        mRectItem->setBrush(QBrush(QColor(255-i*8,i*8,255-i*8)));

        //mRectItem->setBrush(QBrush(QColor(255,0,0)));
        QGraphicsTextItem *mTextItem = new QGraphicsTextItem("0x0000");
        mTextItem->setPos(pPacketMemView->mapFromScene(100,-10 + 8*i));
        QFont mFont;
        mFont.setPixelSize(15);
        mFont.setBold(true);
        mTextItem->setDefaultTextColor(QColor(255,0,0));
        mTextItem->setFont(mFont);
        //pPacketScene->addItem(mTextItem);
        mTextItem->setParentItem(mRectItem);
        mRectItem->setPos(pPacketMemView->mapFromScene(50,100+10*i));
        pPacketScene->addItem(mRectItem);
    }

    /*布局左边的图形显示和信息显示*/
    QVBoxLayout* pVLayout01 = new QVBoxLayout();
    pVLayout01->addWidget(pPacketMemView);
    pVLayout01->addWidget(pPacketFileInfoText);
    pVLayout01->setStretch(0, 3);
    pVLayout01->setStretch(1, 1);

    /*布局加载按钮和转换按钮*/
    QHBoxLayout *pHLayout02 = new QHBoxLayout();
    pHLayout02->addWidget(pPacketLoadBtn);
    pHLayout02->addWidget(pPacketBtn);

    /*布局按钮和文件列表*/
    QVBoxLayout *pVLayout03 = new QVBoxLayout();
    pVLayout03->addLayout(pHLayout02);
    pVLayout03->addWidget(pPacketFileListView);
    pVLayout03->addWidget(pPacketStatus);

    /**加载两个布局*/
    QHBoxLayout *pHLayout04 = new QHBoxLayout();
    pHLayout04->addLayout(pVLayout03);
    pHLayout04->addLayout(pVLayout01);

    /*设置两个布局的显示比例*/
    pHLayout04->setStretch(0, 2);
    pHLayout04->setStretch(1, 1);
    parent->setLayout(pHLayout04);

}

void Packet::paintEvent(QPaintEvent *event)
{
    //    QPainter mMemPainter(this->pPacketMemBoard);
    //    QPen mPen(QColor(255,0,0));
    //    QBrush mBrush (QColor(0,0,255));
    //    mMemPainter.setPen(mPen);
    //    mMemPainter.setBrush(mBrush);
    //    mMemPainter.drawRect(0,-20, 500,500);
    //    mMemPainter.drawPixmap(QPoint(0,0), QPixmap(":/new/prefix2/Image/logo.png"));

    //    QPixmap pix;
    //    pix.load(":/new/prefix2/Image/logo.png");
    //    QPainter p(this);
    //    //QPoint p0 = this->pPacketMemBoard->pos();
    //    p.drawPixmap(0,0,100,100,pix);
}

void Packet::on_files_load_btn_clicked()
{
    qDebug()<<"packet button is clicked";
    QStringList mFileList = QFileDialog::getOpenFileNames(this, tr("选取文件"), "","*.bin;;*.*",0);
    if (mFileList.empty())
        return;
    pPacketListModel->setStringList(mFileList);
    pPacketStatus->setText(tr("已加载")+QString::number(mFileList.size())+tr("个文件."));
}
void Packet::on_packet_btn_clicked()
{
    /*创建文件*/
    QFile file("./imagePacket.bin");
    file.open( QIODevice::WriteOnly );

    qDebug()<<"on_packet_btn_clicked";
    /*提取选中的文件*/
    QStringList fileStrList = pPacketListModel->stringList();

    /*设置头部*/
    QByteArray HeadArr;
    QByteArray HexArr;
    qint64 offset = 0;
    /*文件的个数*/
    HeadArr += (fileStrList.size());
    offset += 1;
    offset += 4*fileStrList.size();

    QListIterator<QString> i(fileStrList);
    while (i.hasNext())
    {
        QFile loadfile(i.next());
        if(loadfile.open(QIODevice::ReadOnly))
        {
            /*将文件偏移量写入*/
            HeadArr += offset &0xff;
            HeadArr += (offset>>8) & 0xff;
            /*将文件大小写入*/
            HeadArr += loadfile.size() & 0xff;
            HeadArr += (loadfile.size()>>8) & 0xff;

            /*偏移文件的大小，写入文件的内容*/
            offset += loadfile.size();
            HexArr  += loadfile.readAll();
            //qDebug() << i.next();
        }
        else
        {
            if ( HeadArr[0] > 0)
                HeadArr[0] = HeadArr[0] - 1;
        }
        loadfile.close();
    }
    file.write(HeadArr);
    file.write(HexArr);
    file.close();

    QMessageBox::information(NULL, tr("恭喜!恭喜!"), tr("已合成 0x")+
                             QString::number(HeadArr[0])+tr(" 个文件"),
            QMessageBox::Ok );
    pPacketStatus->setText(tr("已合成")+QString::number(HeadArr[0])+tr("个文件."));
}

