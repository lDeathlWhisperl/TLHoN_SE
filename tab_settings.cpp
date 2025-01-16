#include "tab_settings.h"
#include "ui_tab_settings.h"

#include "jsonparser.h"
#include "stylefromfile.h"

#include <QFile>
#include <QProcessEnvironment>

int Tab_Settings::characterId;

Tab_Settings::Tab_Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab_Settings)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_Settings.qss");

    auto json = JsonParser::getJson();
    for(auto& obj : JsonParser::getJson())
        ui->cb_saves->addItem(" " + obj["name"].toString());
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
    auto& json = JsonParser::getJson();
    characterId = index;

    ui->le_maxIconUses->blockSignals(true);
    ui->le_maxIconUses->setText(QString::number(json[characterId]["maxIconUses"].toInt()));
    ui->le_maxIconUses->blockSignals(false);

    emit characterChanged();
}

void Tab_Settings::on_btn_restore_clicked()
{
    QString appdata = QProcessEnvironment::systemEnvironment().value("AppData");
    QString path = "\\..\\LocalLow\\OverTheMoon\\TLHON\\SaveData.txt";

    QFile::remove(appdata + path);
    QFile::rename(appdata + path + "_bckp", appdata + path);
    QFile::copy(appdata + path, appdata + path + "_bckp");

    JsonParser::clear();
    JsonParser::init();

    emit saveRestored();
}

void Tab_Settings::on_cb_cheater_toggled(bool checked)
{
    emit modeToggled(checked);
}

void Tab_Settings::on_le_maxIconUses_editingFinished()
{
    auto& json = JsonParser::getJson();
    json[characterId]["maxIconUses"] = ui->le_maxIconUses->text().toInt();

    JsonParser::write();
}
