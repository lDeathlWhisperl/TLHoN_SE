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

Tab_Stats::Tab_Stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_stats)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resourses/StyleSheets/Tab_Stats.qss");

    connect(ui->le_memory,          &QLineEdit::editingFinished, [this]() { le_textChanged("memory",           ui->le_memory->text());          });
    connect(ui->le_stat_vitality,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_vitality",   ui->le_stat_vitality->text());   });
    connect(ui->le_stat_stamina,    &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_stamina",    ui->le_stat_stamina->text());    });
    connect(ui->le_stat_equipLoad,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_equipLoad",  ui->le_stat_equipLoad->text());  });
    connect(ui->le_stat_strength,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_strength",   ui->le_stat_strength->text());   });
    connect(ui->le_stat_dexterity,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_dexterity",  ui->le_stat_dexterity->text());  });
    connect(ui->le_stat_attunement, &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_attunement", ui->le_stat_attunement->text()); });
    connect(ui->le_stat_luck,       &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_luck",       ui->le_stat_luck->text());       });

    update();
}

void Tab_Stats::initSettings(Tab_Settings *s)
{
    connect(s, &Tab_Settings::characterChanged, this, &Tab_Stats::update);
}

void Tab_Stats::le_textChanged(QString param, QString text)
{
    if(isBlockSignal) return;

    auto json = JsonParser::getJson();
    json[id][param] = text.toInt();
    // JsonParser::write();
}

void Tab_Stats::update()
{
    id = Tab_Settings::getCharacterId();
    auto json = JsonParser::getJson();

    isBlockSignal = true;

    ui->l_nickname->setText(json[id]["name"].toString());
    ui->l_class->setText(getClass(json[id]["playerClass"].toInt()));
    ui->l_level->setText(QString::number(json[id]["level"].toInt()));
    ui->l_health->setText(QString::number(floor(json[id]["health"].toDouble())) + "/[MAX]");
    ui->l_stamina->setText("[STAMINA]");
    ui->le_memory->setText(QString::number(json[id]["memory"].toInt()));
    ui->l_required->setText("[REQUIRED]");
    ui->le_stat_vitality->setText(QString::number(json[id]["_stat_vitality"].toInt()));
    ui->le_stat_stamina->setText(QString::number(json[id]["_stat_stamina"].toInt()));
    ui->le_stat_equipLoad->setText(QString::number(json[id]["_stat_equipLoad"].toInt()));
    ui->le_stat_strength->setText(QString::number(json[id]["_stat_strength"].toInt()));
    ui->le_stat_dexterity->setText(QString::number(json[id]["_stat_dexterity"].toInt()));
    ui->le_stat_attunement->setText(QString::number(json[id]["_stat_attunement"].toInt()));
    ui->le_stat_luck->setText(QString::number(json[id]["_stat_luck"].toInt()));

    isBlockSignal = false;
}

Tab_Stats::~Tab_Stats()
{
    delete ui;
}
