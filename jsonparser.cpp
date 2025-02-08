#include "jsonparser.h"

#include <QFile>
#include <QRegularExpression>
#include <QMessageBox>

QList<QJsonObject> JsonParser::characterJson;
QList<QJsonObject> JsonParser::infoJson;
QList<QJsonObject> JsonParser::questJson;
QList<QString> JsonParser::misc;

void JsonParser::init()
{
    appdata = appdata.split("Roaming")[0];

    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", QObject::tr("Файл ")  + appdata + path + QObject::tr(" не найден."));
        qFatal() << "file not found";
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString st = file.readAll();

    if(st.isEmpty())
    {
        QMessageBox::critical(nullptr, "Critical", QObject::tr("Файл пуст. Восстановите его вручную."));
        qFatal() << "File is empty";
    }

    file.close();

    QRegularExpression re("~[A-z]*~");
    QList data = st.split(re);
    misc = {data[0]};
    data.pop_front();

    try
    {
        for(QString& ch_data : data)
        {
            QJsonParseError data_err;
            QJsonParseError info_err;
            QJsonParseError quests_err;

            QString chunk = ch_data;
            QString ch_quests;
            QString ch_info;

            ch_data   = chunk.split(';')[0];
            ch_info   = chunk.split(';')[1];
            ch_quests = chunk.split(';')[2];

            misc.push_back(chunk.section(';', 3));

            QJsonDocument ch_doc = QJsonDocument::fromJson(ch_data.toUtf8(), &data_err);
            QJsonDocument in_doc = QJsonDocument::fromJson(ch_info.toUtf8(), &info_err);
            QJsonDocument qu_doc = QJsonDocument::fromJson(ch_quests.toUtf8(), &quests_err);

            if(ch_doc.isNull())
            {
                qDebug() << " QJsonDocument error: " << data_err.errorString() << info_err.errorString() << quests_err.errorString();
                QMessageBox::critical(nullptr, "Critical", QObject::tr("Ошибка парсинга. Файл сохранения может быть поврежден. Пожалуйста, восстановите его вручную."));
            }

            characterJson.push_back(ch_doc.object());
            infoJson.push_back(in_doc.object());
            questJson.push_back(qu_doc.object());
        }
    }
    catch (std::exception& e)
    {
        qFatal() << e.what();
        QMessageBox::critical(nullptr, "Critical", QObject::tr("Что-то пошло не так :("));
    }

    if(!QFile::exists(appdata + path + "_bckp")) QFile::copy(appdata + path, appdata + path + "_bckp");
}

QList<QJsonObject>& JsonParser::getCharacterJson()
{
    return characterJson;
}

QList<QJsonObject> &JsonParser::getInfoJson()
{
    return infoJson;
}

QList<QJsonObject> &JsonParser::getQuestJson()
{
    return questJson;
}

void JsonParser::write()
{
    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", QObject::tr("Файл ")  + appdata + path + QObject::tr(" не найден."));
        qFatal() << "file not found";
    }

    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append | QIODevice::Truncate);

    file.write(misc[0].toUtf8());
    file.write("~SettingsSplit~");

    try
    {
        for(size_t i = 0; i < characterJson.size(); ++i)
        {
            file.write(QJsonDocument(characterJson[i]).toJson() + ";");
            file.write(QJsonDocument(infoJson[i]).toJson()  + ";");
            file.write(QJsonDocument(questJson[i]).toJson() + ";");
            file.write(misc[i+1].toUtf8());
            if(i != characterJson.size() - 1) file.write("~NoticeMeSenpaiNoticeMe~");
        }
    }
    catch (std::exception& e)
    {
        qFatal() << e.what();
        QMessageBox::critical(nullptr, "Critical", QObject::tr("Что-то пошло не так :("));
    }
}

void JsonParser::clear()
{
    characterJson.clear();
    misc.clear();
}
