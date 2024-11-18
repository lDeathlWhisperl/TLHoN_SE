#include "tab_stats.h"
#include "ui_tab_stats.h"
#include "stylefromfile.h"

#include "jsonparser.h"
#include "tab_settings.h"
/*
void save()
{
    //parsing
    QFile file("E:\\Projects\\Qt\\TLHoN_IC\\Resourses\\Tests\\SaveData.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString st = file.readAll();
    file.close();

    QRegularExpression re("~[A-z]*~");
    QList data = st.split(re);
    QList misc = {data[0]};
    data.pop_front();

    QList<QJsonObject> json;
    for(QString& str : data)
    {
        QJsonParseError err;

        QString chunk = str;
        str = chunk.split(';')[0];
        misc.push_back(chunk.section(';', 1));

        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &err);
        qDebug() << "Error: " << err.errorString();
        json.push_back(doc.object());
    }

    //writing
    file.setFileName("E:\\Projects\\Qt\\TLHoN_IC\\Resourses\\Tests\\SaveData.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append | QIODevice::Truncate);

    file.write(misc[0].toUtf8());
    misc.pop_front();
    file.write("~SettingsSplit~");

    for(size_t i = 0; i < json.size(); ++i)
    {
        file.write(QJsonDocument(json[i]).toJson() + ";");
        file.write(misc[0].toUtf8());
        if(i != json.size() - 1) file.write("~NoticeMeSenpaiNoticeMe~");
    }
    //
}
*/

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

    update();
}

void Tab_Stats::initSetting(Tab_Settings *s)
{
    connect(s, &Tab_Settings::characterChanged, this, &Tab_Stats::update);
}

void Tab_Stats::update()
{
    id = Tab_Settings::getCharacterId();
    auto json = JsonParser::getJson();

    ui->l_nickname->setText(json[id]["name"].toString());
    ui->l_class->setText(getClass(json[id]["playerClass"].toInt()));
    ui->l_level->setText(QString::number(json[id]["level"].toInt()));
    ui->l_health->setText(QString::number(floor(json[id]["health"].toDouble())) + "/[MAX]");
    ui->l_stamina->setText("[STAMINA]");
    ui->l_memory->setText(QString::number(json[id]["memory"].toInt()));
    ui->l_required->setText("[REQUIRED]");
    ui->l_stat_vitality->setText(QString::number(json[id]["_stat_vitality"].toInt()));
    ui->l_stat_stamina->setText(QString::number(json[id]["_stat_stamina"].toInt()));
    ui->l_stat_equipLoad->setText(QString::number(json[id]["_stat_equipLoad"].toInt()));
    ui->l_stat_strength->setText(QString::number(json[id]["_stat_strength"].toInt()));
    ui->l_stat_dexterity->setText(QString::number(json[id]["_stat_dexterity"].toInt()));
    ui->l_stat_attunement->setText(QString::number(json[id]["_stat_attunement"].toInt()));
    ui->l_stat_luck->setText(QString::number(json[id]["_stat_luck"].toInt()));
}

Tab_Stats::~Tab_Stats()
{
    delete ui;
}
