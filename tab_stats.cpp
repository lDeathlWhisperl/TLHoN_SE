#include "tab_stats.h"
#include "ui_tab_stats.h"
#include "stylefromfile.h"

#include "jsonparser.h"
#include "tab_settings.h"

enum
{
    DATADIN,
    FORMATTER,
    RESOLUTIONARY,
    SOURCERER,
    RANDOMASTER
};

QString getClass(int c)
{
    switch(c)
    {
    case DATADIN:
        return "Инфорыцарь";
    case FORMATTER:
        return "Форматировщик";
    case RESOLUTIONARY:
        return "Разбойник";
    case SOURCERER:
        return "Чарокодер";
    default:
        return "Рандомастер";
    }
}

void disableIfCompleted(QCheckBox* cb, int value, int cond)
{
    if(value >= cond)
    {
        cb->setChecked(true);
        cb->setEnabled(false);
    }
}

Tab_Stats::Tab_Stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_stats)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_Stats.qss");

    connect(ui->le_memory,          &QLineEdit::editingFinished, [this]() { le_textChanged("memory",             ui->le_memory->text());          });
    connect(ui->le_stat_vitality,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_vitality",     ui->le_stat_vitality->text());   });
    connect(ui->le_stat_stamina,    &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_stamina",      ui->le_stat_stamina->text());    });
    connect(ui->le_stat_equipLoad,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_equipLoad",    ui->le_stat_equipLoad->text());  });
    connect(ui->le_stat_strength,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_strength",     ui->le_stat_strength->text());   });
    connect(ui->le_stat_dexterity,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_dexterity",    ui->le_stat_dexterity->text());  });
    connect(ui->le_stat_attunement, &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_attunement",   ui->le_stat_attunement->text()); });
    connect(ui->le_stat_luck,       &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_luck",         ui->le_stat_luck->text());       });

    connect(ui->cb_backstabs,  &QCheckBox::toggled, [this](bool) { setCheat("backstabCount",      20); });
    connect(ui->cb_repostes,   &QCheckBox::toggled, [this](bool) { setCheat("reposteCount",       20); });
    connect(ui->cb_rebuffs,    &QCheckBox::toggled, [this](bool) { setCheat("rebuffCount",        20); });
    connect(ui->cb_remembered, &QCheckBox::toggled, [this](bool) { setCheat("itemsRemembered",    75); });
    connect(ui->cb_unburdened, &QCheckBox::toggled, [this](bool) { setCheat("itemsHoovered",      71); });
    connect(ui->cb_DLC,        &QCheckBox::toggled, [this](bool) { setCheat("DLCItemsRemembered", 10); });

    ui->btn_cheat->setVisible(false);
    ui->line_3->setVisible(false);

    for(int i = 0; i < ui->vl_cheat_layout->count(); ++i)
        if(auto w = ui->vl_cheat_layout->itemAt(i)->widget(); w)
            w->setVisible(false);

    ui->btn_cheat->setCheckable(true);
    update();
}

void Tab_Stats::initSettings(Tab_Settings *s)
{
    connect(s, &Tab_Settings::characterChanged, this, &Tab_Stats::update);
    connect(s, &Tab_Settings::modeToggled, ui->btn_cheat, &QPushButton::setVisible);
    connect(s, &Tab_Settings::saveRestored, this, &Tab_Stats::update);
}

void Tab_Stats::le_textChanged(QString param, QString text)
{
    if(isBlockSignal) return;

    auto& json = JsonParser::getJson();
    json[id][param] = text.toInt();

    JsonParser::write();
}

void Tab_Stats::setCheat(QString param, int value)
{
    if(isBlockSignal) return;

    auto& json = JsonParser::getJson();
    json[id][param] = value;

    JsonParser::write();

    update();
}

void Tab_Stats::update()
{
    id = Tab_Settings::getCharacterId();
    auto& json = JsonParser::getJson();

    isBlockSignal = true;

    ui->l_nickname->setText(json[id]["name"].toString());

    ui->l_class->setText(getClass(json[id]["playerClass"].toInt()));
    ui->l_level->setText(QString::number(json[id]["level"].toInt()));
    ui->l_health->setText(QString::number(floor(json[id]["health"].toDouble())) + "/[NULL]");
    ui->l_stamina->setText("[NULL]");

    ui->le_memory->setText(QString::number(json[id]["memory"].toInt()));
    ui->l_required->setText("[NULL]");

    ui->le_stat_vitality->setText(QString::number(json[id]["_stat_vitality"].toInt()));
    ui->le_stat_stamina->setText(QString::number(json[id]["_stat_stamina"].toInt()));
    ui->le_stat_equipLoad->setText(QString::number(json[id]["_stat_equipLoad"].toInt()));
    ui->le_stat_strength->setText(QString::number(json[id]["_stat_strength"].toInt()));
    ui->le_stat_dexterity->setText(QString::number(json[id]["_stat_dexterity"].toInt()));
    ui->le_stat_attunement->setText(QString::number(json[id]["_stat_attunement"].toInt()));
    ui->le_stat_luck->setText(QString::number(json[id]["_stat_luck"].toInt()));

    ui->l_faded->setText("[NULL]");
    ui->l_remembered->setText(QString::number(json[id]["itemsRemembered"].toInt()));
    ui->l_unburdened->setText(QString::number(json[id]["itemsHoovered"].toInt()));
    ui->l_DLC->setText(QString::number(json[id]["DLCItemsRemembered"].toInt()));

    disableIfCompleted(ui->cb_backstabs,   json[id]["backstabCount"].toInt(),      20);
    disableIfCompleted(ui->cb_repostes,    json[id]["reposteCount"].toInt(),       20);
    disableIfCompleted(ui->cb_rebuffs,     json[id]["rebuffCount"].toInt(),        20);
    disableIfCompleted(ui->cb_remembered,  json[id]["itemsRemembered"].toInt(),    75);
    disableIfCompleted(ui->cb_unburdened,  json[id]["itemsHoovered"].toInt(),      71);
    disableIfCompleted(ui->cb_DLC,         json[id]["DLCItemsRemembered"].toInt(), 10);

    isBlockSignal = false;
}

Tab_Stats::~Tab_Stats()
{
    delete ui;
}

void Tab_Stats::on_btn_cheat_toggled(bool checked)
{
    if(checked) ui->btn_cheat->setText("<--");
    else ui->btn_cheat->setText("-->");
    ui->line_3->setVisible(checked);

    for(int i = 0; i < ui->vl_cheat_layout->count(); ++i)
        if(auto w = ui->vl_cheat_layout->itemAt(i)->widget(); w)
            w->setVisible(checked);
}

