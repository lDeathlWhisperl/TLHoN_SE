#include "tab_settings.h"
#include "ui_tab_settings.h"

#include "jsonparser.h"

int Tab_Settings::characterId;

Tab_Settings::Tab_Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab_Settings)
{
    ui->setupUi(this);

    auto json = JsonParser::getJson();
    for(auto& obj : JsonParser::getJson())
        ui->cb_saves->addItem(obj["name"].toString());
}

int Tab_Settings::getCharacterId()
{
    return characterId;
}

Tab_Settings::~Tab_Settings()
{
    delete ui;
}

void Tab_Settings::on_cb_saves_currentIndexChanged(int index)
{
    characterId = index;
    emit characterChanged();
}

