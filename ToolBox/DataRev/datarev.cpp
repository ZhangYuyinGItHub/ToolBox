#include "datarev.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QPushButton>

DataRev::DataRev(QWidget *parent) : QWidget(parent)
{

    this->setStyleSheet("QLabel{background-color: rgb(100, 180, 140);"
                        "border:0.6px solid blue;"
                        "min-height: 24px; "
                        "font: 13pt solid}"
                        "QPushButton{background-color: rgb(199, 199, 199);"
                        "border-radius: 3px;"
                        "min-height: 30px; "
                        "min-width: 80px;  "
                        "font: 11pt;"
                        "/*border: 1px groove gray;*/}"
                        "QLineEdit{min-height: 30px; font: 13pt;}"
                        "QComboBox{min-height: 30px; font: 13pt;}"
                        "QPushButton:hover {background-color: lime;}"
                        "QPushButton:pressed{background-color:rgb(255, 170, 170);"
                        "border-style: inset;}");

    QVBoxLayout *pLayout0 = new QVBoxLayout(this);

    QVBoxLayout *pItemFlowLayout = new QVBoxLayout();
    pItemFlowLayout->setSpacing(2);
    QLabel *pKeyStartCmdLabel = new QLabel("key start cmd");
    QLabel *pSlaveStartCmdLabel = new QLabel("Slave start Cmd");
    QLabel *pMasterStartCmdLabel = new QLabel("Mater start Cmd");
    QLabel *pDataRevLabel = new QLabel("Data Rev");
    QLabel *pSlaveEndCmdLabel = new QLabel("Slave end Cmd");
    QLabel *pMasterEndCmdLabel = new QLabel("Master end Cmd");
    QLabel *pKeyEndCmdLabel = new QLabel("Key end Cmd");

    pItemFlowLayout->addWidget(pKeyStartCmdLabel);
    pItemFlowLayout->addWidget(pSlaveStartCmdLabel);
    pItemFlowLayout->addWidget(pMasterStartCmdLabel);
    pItemFlowLayout->addWidget(pDataRevLabel);
    pItemFlowLayout->addWidget(pSlaveEndCmdLabel);
    pItemFlowLayout->addWidget(pMasterEndCmdLabel);
    pItemFlowLayout->addWidget(pKeyEndCmdLabel);

    //config name
    QLabel *pItemNameLabel = new QLabel("name");
    QLineEdit *pNameEdit = new QLineEdit();
    QVBoxLayout *pNameLayout = new QVBoxLayout();
    pNameLayout->addWidget(pItemNameLabel);
    pNameLayout->addWidget(pNameEdit);

    //config direct
    QLabel *pItemDirectLabel = new QLabel("name");
    QComboBox *pItemDirectBox = new QComboBox();
    pItemDirectBox->addItem(" M-->S ");
    pItemDirectBox->addItem(" S-->M ");
    QVBoxLayout *pItemDirectLayout = new QVBoxLayout();
    pItemDirectLayout->addWidget(pItemDirectLabel);
    pItemDirectLayout->addWidget(pItemDirectBox);

    QHBoxLayout *pLayout001 = new QHBoxLayout();
    pLayout001->addLayout(pNameLayout);
    pLayout001->addLayout(pItemDirectLayout);

    //config type
    QLabel *pItemTypeLabel = new QLabel("Type");
    QComboBox *pItemTypeBox = new QComboBox();
    pItemTypeBox->addItem(" Key Type ");
    pItemTypeBox->addItem(" Cmd Type ");
    pItemTypeBox->addItem(" Data Type ");
    QVBoxLayout *pTypeLayout = new QVBoxLayout();
    pTypeLayout->addWidget(pItemTypeLabel);
    pTypeLayout->addWidget(pItemTypeBox);

    //config value
    QLabel *pItemValueLabel = new QLabel("Value");
    QLineEdit *pValueEdit = new QLineEdit();
    QVBoxLayout *pValueLayout = new QVBoxLayout();
    pValueLayout->addWidget(pItemValueLabel);
    pValueLayout->addWidget(pValueEdit);

    QHBoxLayout *pLayout002 = new QHBoxLayout();
    pLayout002->addLayout(pTypeLayout);
    pLayout002->addLayout(pValueLayout);

    QVBoxLayout *pItemConfigLayout = new QVBoxLayout();
    pItemConfigLayout->addLayout(pLayout001);
    pItemConfigLayout->addLayout(pLayout002);

    QPlainTextEdit *pMessageEdit = new QPlainTextEdit();

    QHBoxLayout *pLayout003 = new QHBoxLayout();
    QPushButton *pConfigSaveBtn = new QPushButton("Save");
    QPushButton *pConfigDeleteBtn = new QPushButton("Delete");
    pLayout003->addWidget(pConfigSaveBtn);
    pLayout003->addWidget(pConfigDeleteBtn);
    pItemConfigLayout->addLayout(pLayout003);
    pItemConfigLayout->addWidget(pMessageEdit);

    QHBoxLayout *pLayout01 = new QHBoxLayout();
    pLayout01->addLayout(pItemFlowLayout);
    pLayout01->addLayout(pItemConfigLayout);

    pLayout0->addLayout(pLayout01);
}
