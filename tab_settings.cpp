#include "tab_settings.h"
#include "ui_tab_settings.h"

#include "jsonparser.h"
#include "stylefromfile.h"

#include <QFile>
#include <QProcessEnvironment>
#include <QSettings>

int Tab_Settings::characterId;
QString Tab_Settings::language = "en";

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

QString Tab_Settings::getLanguage()
{
    return language;
}

void Tab_Settings::initLanguage()
{
    on_cb_language_currentIndexChanged(loadSettings("language").toInt());
    ui->cb_language->blockSignals(true);
    ui->cb_language->setCurrentIndex(loadSettings("language").toInt());
    ui->cb_language->blockSignals(false);
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

    int max = ui->le_maxIconUses->text().toInt();
    if(max < 1)
    {
        max = 1;
        ui->le_maxIconUses->blockSignals(true);
        ui->le_maxIconUses->setText("1");
        ui->le_maxIconUses->blockSignals(true);
    }

    json[characterId]["maxIconUses"] = max;

    JsonParser::write();
}

void Tab_Settings::on_cb_language_currentIndexChanged(int index)
{
    qApp->removeTranslator(&translator);
    QString file;

    switch (index)
    {
    case 1:
        file = ":/translations/TLHoN_OC_ru.qm";
        language = "ru";
        break;
    default:
        file = ":/translations/TLHoN_OC_en.qm";
        language = "en";
        break;
    }

    saveSettings("Language", QString::number(index));

    if(translator.load(file))
        qApp->installTranslator(&translator);

    emit ui->retranslateUi(this);
    emit languageChanged();

    on_cb_saves_currentIndexChanged(characterId);
}

void Tab_Settings::saveSettings(const QString &key, const QString &value)
{
    QSettings settings("DeathWhisper", "TLHoN_IC");
    settings.setValue(key, value);
}

QString Tab_Settings::loadSettings(const QString &key)
{
    QSettings settings("DeathWhisper", "TLHoN_IC");
    return settings.value(key).toString();
}
