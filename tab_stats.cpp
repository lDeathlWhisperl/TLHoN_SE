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
    QMap<int, QMap<QString, QString>> classes;

    classes[DATADIN]      ["ru"] = "Инфорыцарь";
    classes[FORMATTER]    ["ru"] = "Форматировщик";
    classes[RESOLUTIONARY]["ru"] = "Разбойник";
    classes[SOURCERER]    ["ru"] = "Чарокодер";
    classes[RANDOMASTER]  ["ru"] = "Рандомастер";

    classes[DATADIN]      ["en"] = "DATADIN";
    classes[FORMATTER]    ["en"] = "FORMATTER";
    classes[RESOLUTIONARY]["en"] = "RESOLUTIONARY";
    classes[SOURCERER]    ["en"] = "SOURCERER";
    classes[RANDOMASTER]  ["en"] = "RANDOMASTER";

    return classes[c][Tab_Settings::getLanguage()];
}

void disableIfCompleted(QCheckBox* cb, int value, int cond)
{
    if(value >= cond)
    {
        cb->setChecked(true);
        cb->setEnabled(false);
    }
    else
    {
        cb->setChecked(false);
        cb->setEnabled(true);
    }
}

QJsonObject createNewItem(const QString &guid, int uses = -1, bool isAwake = false, bool isHoovered = false, bool hasSeen = true)
{
    QJsonObject newItem;
    newItem["GUID"]                 = guid;
    newItem["uses"]                 = uses;
    newItem["isAwake"]              = isAwake;
    newItem["isHoovered"]           = isHoovered;
    newItem["hasSeen"]              = hasSeen;
    newItem["instanceIDOnCreation"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
    newItem["newGamePlusCount"]     = 0;
    return newItem;
}

Tab_Stats::Tab_Stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_stats)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_Stats.qss");

    connect(ui->le_memory,          &QLineEdit::editingFinished, [this]() { le_textChanged("memory",           ui->le_memory->text());          });
    connect(ui->le_stat_vitality,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_vitality",   ui->le_stat_vitality->text());   });
    connect(ui->le_stat_stamina,    &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_stamina",    ui->le_stat_stamina->text());    });
    connect(ui->le_stat_equipLoad,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_equipLoad",  ui->le_stat_equipLoad->text());  });
    connect(ui->le_stat_strength,   &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_strength",   ui->le_stat_strength->text());   });
    connect(ui->le_stat_dexterity,  &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_dexterity",  ui->le_stat_dexterity->text());  });
    connect(ui->le_stat_attunement, &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_attunement", ui->le_stat_attunement->text()); });
    connect(ui->le_stat_luck,       &QLineEdit::editingFinished, [this]() { le_textChanged("_stat_luck",       ui->le_stat_luck->text());       });

    connect(ui->cb_backstabs,  &QCheckBox::toggled, [this](bool) { setCheat("backstabCount",      20); });
    connect(ui->cb_repostes,   &QCheckBox::toggled, [this](bool) { setCheat("reposteCount",       20); });
    connect(ui->cb_rebuffs,    &QCheckBox::toggled, [this](bool) { setCheat("rebuffCount",        20); });
    connect(ui->cb_remembered, &QCheckBox::toggled, [this](bool) { setCheat("itemsRemembered",    75); });
    connect(ui->cb_unburdened, &QCheckBox::toggled, [this](bool) { setCheat("itemsHoovered",      71); });
    connect(ui->cb_DLC,        &QCheckBox::toggled, [this](bool) { setCheat("DLCItemsRemembered", 10); });

    const QList<QString> spells =
        {
            "Spell_WepInfusion_StealHealth",
            "Spell_BombCode",
            "Spell_ThrowablePixelStorm",
            "Spell_RecursionDamage",
            "Spell_KeepMemoryOnDeath",
            "Spell_SpawnDecoy",
            "Spell_TrapTarget",
            "Spell_EscapePath",
            "Spell_WepInfusion_Glitch",
            "Spell_StunSurrounding",
            "Spell_StartingMagicMissile",
            "Spell_ProjectileDOT",
            "Spell_HOT",
            "Spell_SlowPixelBomb",
            "Spell_LightningChain",
            "Spell_Thorns",
            "Throwable_FadedSkuriken"
        };  
    const QList<QString> runes =
        {
            "Relic_DecreaseProjectileStagger",
            "Relic_RollIFramesUp",
            "Relic_StatUp_Strength",
            "Relic_PoiseUp",
            "Relic_MemoryDropIncrease",
            "Relic_CritChanceUp",
            "Relic_StaminaRegen",
            "Relic_MagicDefenseUp",
            "Relic_StatUp_Dex",
            "Relic_KiteParryBoost",
            "Relic_SwordParryBoost",
            "Relic_StatUp_Mag",
            "Relic_MagicThorns",
            "Relic_Damage_Abominations",
            "Relic_Damage_Beasts",
            "Relic_Damage_Evil",
            "Relic_Damage_Humanoids",
            "Relic_HealthRegenOnCrit",
            "Relic_DecreaseStaminaUsage",
            "Relic_IncreaseHeroicIconPotency",
            "Relic_AmmoCapacityUp",
            "Relic_RecursionDamageUp",
            "Relic_EquipLoadUp",
            "Relic_ReduceBuildUp",
            "Relic_HealthBelowAtkBoost",
        };
    const QList<QString> armor =
        {
        "SkeletonArmor_Helm",
        "SkeletonArmor_Torso",
        "SkeletonArmor_Legs",
        "SkeletonArmor_Arms",
        "Armour_Chainmail_Helm",
        "Armour_Chainmail_Torso",
        "Armour_Chainmail_Legs",
        "Armour_Chainmail_Arms",
        "AntiHero_Helm",
        "AntiHero_Torso",
        "AntiHero_Legs",
        "AntiHero_Arms",
        "MasterChefArmor_Helm",
        "MasterChefArmor_Torso",
        "MasterChefArmor_Legs",
        "MasterChefArmor_Arms",
        "CrownOfTheUndone_Helm",
        "TargetArmor_Torso",
        "ChildMask_Helm",
        "GarrettArmor_Helm",
        "GarrettArmor_Torso",
        "GarrettArmor_Legs",
        "GarrettArmor_Arms",
        "WorryWatchers_Helm",
        "JacketArmor_Helm",
        "JacketArmor_Torso",
        "JacketArmor_Legs",
        "JacketArmor_Arms",
        "TerraArmor_Helm",
        "TerraArmor_Torso",
        "TerraArmor_Legs",
        "TerraArmor_Arms",
        "RedRacer_Legs",
        "WelcomeArmour_TopHat",
        "PixelKnight_Helm",
        "PixelKnight_Torso",
        "PixelKnight_Legs",
        "PixelKnight_Arms",
        "LeisureSuit_Legs",
        "LeatherArmour_Torso",
        "LeatherArmour_Legs",
        "LeatherArmour_Arms",
        "BikiniArmor_Torso",
        "BikiniArmor_Legs",
        "IsaacArmor_Helm",
        "IsaacArmor_Torso",
        "IsaacArmor_Legs",
        "IsaacArmor_Arms",
        "AloyArmor_Helm",
        "AloyArmor_Torso",
        "AloyArmor_Legs",
        "AloyArmor_Arms",
        "ForestKid_Helm",
        "DirkArmor_Helm",
        "DirkArmor_Torso",
        "DirkArmor_Legs",
        "DirkArmor_Arms",
        "NationalistArmour_Helm",
        "MercuccioArmor_Helm",
        "MercuccioArmor_Torso",
        "MercuccioArmor_Legs",
        "MercuccioArmor_Arms",
        "PapersPleaseArmor_Helm",
        "PapersPleaseArmor_Torso",
        "PapersPleaseArmor_Legs",
        "PapersPleaseArmor_Arms"
        };

    connect(ui->cb_all_spell, &QCheckBox::toggled, [this, spells](bool) { all_item_cheat(ui->cb_all_spell, spells, "(Spell|FadedSkuriken)"); });
    connect(ui->cb_all_rune,  &QCheckBox::toggled, [this, runes ](bool) { all_item_cheat(ui->cb_all_rune,  runes,  "Relic_"); });
    //TODO: FIND ALL ARMOR
    // connect(ui->cb_all_armor, &QCheckBox::toggled, [this, armor ](bool) { all_item_cheat(ui->cb_all_armor, armor,  "(Helm|Torso|Legs|Arms|TopHat)"); });

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
    connect(s, &Tab_Settings::languageChanged, [this](){ui->retranslateUi(this); });
}

void Tab_Stats::le_textChanged(QString param, QString text)
{
    qInfo() << param << " = " << text;
    if(isBlockSignal) return;

    auto& json = JsonParser::getJson();
    json[id][param] = text.toInt();

    JsonParser::write();
}

void Tab_Stats::setCheat(QString param, int value)
{
    qInfo() << param << " = " << value;
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
    ui->l_health->setText(QString::number(floor(json[id]["health"].toDouble())));
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

    //TEMP
    ui->cb_all_weapon->setHidden(true);
    //
}

void Tab_Stats::all_item_cheat(QCheckBox* cb, const QList<QString>& items, const QString& regex)
{
    qInfo() << "regex: " << regex;
    auto& json = JsonParser::getJson();
    QJsonArray inventory = json[id]["inventory"].toArray();
    QList<QString> copy = items;

    for(size_t i = 0; i < inventory.size(); ++i)
        if(const auto& item = inventory[i].toObject()["GUID"].toString(); item.contains(QRegularExpression(regex)))
            copy.removeOne(item);

    if(copy.empty())
    {
        cb->blockSignals(true);
        cb->setChecked(true);
        cb->blockSignals(false);
        qDebug() << "You already have it all";
        return;
    }

    for(const auto& item : copy)
        inventory.append(createNewItem(item));

    json[id]["inventory"] = inventory;
    JsonParser::write();
}
