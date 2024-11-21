#include "tab_equipment.h"
#include "ui_tab_equipment.h"
#include "stylefromfile.h"

Tab_Equipment::Tab_Equipment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_Equipment)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_Equipment.qss");
}

Tab_Equipment::~Tab_Equipment()
{
    delete ui;
}
