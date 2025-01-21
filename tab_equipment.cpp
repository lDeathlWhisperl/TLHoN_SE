#include "tab_equipment.h"
#include "ui_tab_equipment.h"
#include "stylefromfile.h"
#include "tab_settings.h"

Tab_Equipment::Tab_Equipment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_Equipment)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_Equipment.qss");
}

void Tab_Equipment::initSettings(Tab_Settings *s)
{
    connect(s, &Tab_Settings::languageChanged, [this](){ ui->retranslateUi(this); });
}

Tab_Equipment::~Tab_Equipment()
{
    delete ui;
}
