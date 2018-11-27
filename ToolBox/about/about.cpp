#include "about.h"
#include "QHBoxLayout"
#include "QLabel"
#include "QPicture"
#include "QVBoxLayout"

about::about(QWidget *parent) : QWidget(parent)
{
     QVBoxLayout *playout = new QVBoxLayout(this);

     QLabel *pLabel = new QLabel("ToolBox 2.0\n2018-11-27\n");
     pLabel->setAlignment(Qt::AlignCenter);

     QHBoxLayout *playout01 = new QHBoxLayout();
     QLabel *pLogo = new QLabel();
     QPixmap *pix = new QPixmap(":/new/prefix2/Image/bee.png");
     pLogo->setAlignment(Qt::AlignCenter);
     pLogo->setPixmap(*pix);

     //playout01->addStretch(1);
     playout01->addWidget(pLogo);
     //playout01->addStretch(1);
     playout01->addSpacing(6);

     playout->addWidget(pLabel, 10);
     playout->addLayout(playout01, 1);
     //playout->addStretch(10);
     playout->setSpacing(6);
}
