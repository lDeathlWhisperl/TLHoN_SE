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

    //TEMP
    ui->cb_all_weapon->setHidden(true);
    //
}

void Tab_Stats::on_cb_all_spell_toggled(bool checked)
{
    auto& json = JsonParser::getJson();
    QJsonArray inventory = json[id]["inventory"].toArray();

    QList<QString> spells =
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

    for(size_t i = 0; i < inventory.size(); ++i)
        if(const auto& item = inventory[i].toObject()["GUID"].toString(); item.contains(QRegularExpression("(Spell|FadedSkuriken)")))
            spells.removeOne(item);

    if(spells.empty())
    {
        ui->cb_all_spell->blockSignals(true);
        ui->cb_all_spell->setChecked(true);
        ui->cb_all_spell->blockSignals(false);
        qDebug() << "You already have all spells";
        return;
    }

    for(const auto& spell : spells)
        inventory.append(createNewItem(spell));

    json[id]["inventory"] = inventory;
    JsonParser::write();
}

void Tab_Stats::on_cb_all_armor_toggled(bool checked)
{
    // auto& json = JsonParser::getJson();
    // QJsonArray inventory = json[id]["inventory"].toArray();

    // inventory.append(createNewItem("SkeletonArmor_Helm",      -1, "f0375311-df09-483b-a387-a69473ad733b"));
    // inventory.append(createNewItem("SkeletonArmor_Torso",     -1, "5cf1fc44-24de-4ad5-8ff2-d70f0e18aebd"));
    // inventory.append(createNewItem("SkeletonArmor_Legs",      -1, "9e647e5c-e409-45c2-bf9d-91128d09b449"));
    // inventory.append(createNewItem("SkeletonArmor_Arms",      -1, "71fa1922-be35-4dec-afb7-2cce0eed97eb"));
    // inventory.append(createNewItem("Armour_Chainmail_Helm",   -1, "1cee1203-52d0-41f7-9229-cf6125f8919c"));
    // inventory.append(createNewItem("Armour_Chainmail_Torso",  -1, "19f2de79-fed1-4711-b07c-6d859f1678f7"));
    // inventory.append(createNewItem("Armour_Chainmail_Legs",   -1, "92e0d2b1-fe11-4dd3-9b60-aa1eb1fae76b"));
    // inventory.append(createNewItem("Armour_Chainmail_Arms",   -1, "405592d2-0f6f-4174-aa95-c68666961ce8"));
    // inventory.append(createNewItem("AntiHero_Helm",           -1, "8129549a-dbc9-4719-9531-9ee35d00cf11"));
    // inventory.append(createNewItem("AntiHero_Torso",          -1, "55904df1-db00-43a5-9cbc-879819bf1767"));
    // inventory.append(createNewItem("AntiHero_Legs",           -1, "1e47ef61-ec8b-472e-a2d6-c3eedb24c927"));
    // inventory.append(createNewItem("AntiHero_Arms",           -1, "f07ddd6c-fec3-471d-907a-2ff215d4f764"));
    // inventory.append(createNewItem("MasterChefArmor_Helm",    -1, "6f1c156c-c7c5-4458-a141-43afcc3a6b29"));
    // inventory.append(createNewItem("MasterChefArmor_Torso",   -1, "c78d1a30-d5a5-4bcc-96e3-8cf0d40ad6f8"));
    // inventory.append(createNewItem("MasterChefArmor_Legs",    -1, "71c10131-043f-4d5f-a239-1e3b5db04e5f"));
    // inventory.append(createNewItem("MasterChefArmor_Arms",    -1, "6f1e7023-57cb-4e19-aee5-c2599325da96"));
    // inventory.append(createNewItem("CrownOfTheUndone_Helm",   -1, "39e099c9-7a71-41b2-a44f-76fd4a73dbaf"));
    // inventory.append(createNewItem("TargetArmor_Torso",       -1, "9710e0b7-ac3c-4f9f-a926-fca323b7b652"));
    // inventory.append(createNewItem("ChildMask_Helm",          -1, "cad261ef-3b38-4b69-9ee2-6bd0ed4374ea"));
    // inventory.append(createNewItem("GarrettArmor_Helm",       -1, "2d9f32fe-0283-4532-911e-55c8cdbabec1"));
    // inventory.append(createNewItem("GarrettArmor_Torso",      -1, "90179da1-92b5-4e96-8446-16ca5d4605e7"));
    // inventory.append(createNewItem("GarrettArmor_Legs",       -1, "97c0e907-bf94-4e81-b2f5-ae51334ea6d8"));
    // inventory.append(createNewItem("GarrettArmor_Arms",       -1, "45bc1d29-2aea-4f31-a041-a15a8548d950"));
    // inventory.append(createNewItem("WorryWatchers_Helm",      -1, "8f9de72d-3cf4-43e6-baf5-88df46554684"));
    // inventory.append(createNewItem("JacketArmor_Helm",        -1, "55162a37-4b0f-4b08-95a7-ef2ef305eedc"));
    // inventory.append(createNewItem("JacketArmor_Torso",       -1, "f025cb4f-cb8d-44ad-ac3a-1b39097fdb3b"));
    // inventory.append(createNewItem("JacketArmor_Legs",        -1, "8713ab39-6fcf-4204-92cf-3ed45a5a4694"));
    // inventory.append(createNewItem("JacketArmor_Arms",        -1, "9f209ca9-f01f-452e-891f-cc44120f40e1"));
    // inventory.append(createNewItem("TerraArmor_Helm",         -1, "8f44ad11-9acd-47b0-a5b3-7bda2666fcca"));
    // inventory.append(createNewItem("TerraArmor_Torso",        -1, "657b8604-f22f-4686-a195-6657d74f68ba"));
    // inventory.append(createNewItem("TerraArmor_Legs",         -1, "515ac888-f6a3-4129-b848-d14d4a232055"));
    // inventory.append(createNewItem("TerraArmor_Arms",         -1, "e5365c63-211a-4298-9be5-d82edc9079b1"));
    // inventory.append(createNewItem("RedRacer_Legs",           -1, "fde35206-017b-4f5f-8a47-0378144369f5"));
    // inventory.append(createNewItem("WelcomeArmour_TopHat",    -1, "453d4831-123c-440b-920a-3222c56f602f"));
    // inventory.append(createNewItem("PixelKnight_Helm",        -1, "966f6f4e-6905-46ec-a49b-8dd4e4869222"));
    // inventory.append(createNewItem("PixelKnight_Torso",       -1, "ce7f286a-e1da-4ea8-940a-0a3dcdc959e4"));
    // inventory.append(createNewItem("PixelKnight_Legs",        -1, "776be9cc-1304-4e4b-876b-754eb43d2ad4"));
    // inventory.append(createNewItem("PixelKnight_Arms",        -1, "3614121e-acfb-4b35-ae4d-17ae904dfdda"));
    // inventory.append(createNewItem("LeisureSuit_Legs",        -1, "a1abc5ba-3bc4-4192-8e57-d7bdc4c5795e"));
    // inventory.append(createNewItem("LeatherArmour_Torso",     -1, "3afe05cb-3625-445c-ae2b-3cf83cbdd874"));
    // inventory.append(createNewItem("LeatherArmour_Legs",      -1, "d7aaf5b6-be9e-4dc8-b4b5-0a08a4e0f80c"));
    // inventory.append(createNewItem("LeatherArmour_Arms",      -1, "f6034e6a-a087-4663-becb-ba094d89bdec"));
    // inventory.append(createNewItem("BikiniArmor_Torso",       -1, "185df85b-a95e-4027-b325-058fcfdf63d9"));
    // inventory.append(createNewItem("BikiniArmor_Legs",        -1, "4d8d889c-1aaf-4d38-9f72-9c07b29342ed"));
    // inventory.append(createNewItem("IsaacArmor_Helm",         -1, "e7aeaf49-7ded-46e1-8ca6-2a7d08a7721b"));
    // inventory.append(createNewItem("IsaacArmor_Torso",        -1, "14f5c973-bb1d-47e2-a72f-18fa1b5e20bb"));
    // inventory.append(createNewItem("IsaacArmor_Legs",         -1, "9b5c4e2f-2d5e-4a2b-977d-4948aadedecf"));
    // inventory.append(createNewItem("IsaacArmor_Arms",         -1, "30552226-620a-4c7c-acb9-7f231270c0df"));
    // inventory.append(createNewItem("AloyArmor_Helm",          -1, "7dea414f-fe73-4549-b4a8-93dbc0ce756a"));
    // inventory.append(createNewItem("AloyArmor_Torso",         -1, "de477e66-87d5-4864-a1cc-f2f8823e0219"));
    // inventory.append(createNewItem("AloyArmor_Legs",          -1, "098317fa-7499-4c51-98e3-751579962646"));
    // inventory.append(createNewItem("AloyArmor_Arms",          -1, "ee70d2bf-7205-458b-a04c-4170f9bb9cd9"));
    // inventory.append(createNewItem("ForestKid_Helm",          -1, "8ceb1755-f62c-487e-87c8-729173082d08"));
    // inventory.append(createNewItem("DirkArmor_Helm",          -1, "146928b0-dc57-4de8-93dd-6ad61a9b674c"));
    // inventory.append(createNewItem("DirkArmor_Torso",         -1, "a2f94486-2dcc-4330-94ac-3e7d7d6b9752"));
    // inventory.append(createNewItem("DirkArmor_Legs",          -1, "ee648c06-33fb-4ff7-975a-3662875de686"));
    // inventory.append(createNewItem("DirkArmor_Arms",          -1, "5c4fff71-cf0c-48c9-ae17-717ab99398c5"));
    // inventory.append(createNewItem("NationalistArmour_Helm",  -1, "7f939889-0320-4afe-8289-ca8804f1fb3f"));
    // inventory.append(createNewItem("MercuccioArmor_Helm",     -1, "336abce8-d0e3-4088-a078-e5d7a81d0010"));
    // inventory.append(createNewItem("MercuccioArmor_Torso",    -1, "dc8c2dd3-9d91-40eb-925f-bd42a5a4e792"));
    // inventory.append(createNewItem("MercuccioArmor_Legs",     -1, "fcc0936b-a9fe-48a9-9adb-232f48ab220e"));
    // inventory.append(createNewItem("MercuccioArmor_Arms",     -1, "19598710-730b-429a-917d-4fe915762eca"));
    // inventory.append(createNewItem("PapersPleaseArmor_Helm",  -1, "c5e83333-cf48-48f7-aec8-62699eaac839"));
    // inventory.append(createNewItem("PapersPleaseArmor_Torso", -1, "88440795-49e9-4141-90e1-e45258b44859"));
    // inventory.append(createNewItem("PapersPleaseArmor_Legs",  -1, "ee3bc918-4187-4f49-b236-93015720e857"));
    // inventory.append(createNewItem("PapersPleaseArmor_Arms",  -1, "394e2e71-5cce-49e1-9d6a-7d6cae6c482a"));

    // qDebug() << inventory.size();
    // json[id]["inventory"] = inventory;
    // JsonParser::write();
    // ui->cb_all_armor->setEnabled(false);
}

void Tab_Stats::on_cb_all_rune_toggled(bool checked)
{
    auto& json = JsonParser::getJson();
    QJsonArray inventory = json[id]["inventory"].toArray();

    QList<QString> runes =
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

    for(size_t i = 0; i < inventory.size(); ++i)
        if(const auto& item = inventory[i].toObject()["GUID"].toString(); item.contains("Relic_"))
            runes.removeOne(item);

    if(runes.empty())
    {
        ui->cb_all_rune->blockSignals(true);
        ui->cb_all_rune->setChecked(true);
        ui->cb_all_rune->blockSignals(false);
        qDebug() << "You already have all runes";
        return;
    }

    for(const auto& rune : runes)
        inventory.append(createNewItem(rune));

    json[id]["inventory"] = inventory;
    JsonParser::write();
}
