#include "jsonparser.h"

#include <QFile>
#include <QRegularExpression>
#include <QMessageBox>

QList<QJsonObject> JsonParser::json;
QList<QString> JsonParser::misc;

int JsonParser::init()
{
    appdata = appdata.split("Roaming")[0];

    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", "Файл "  + appdata + path + " не был найден.");
        return 1;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString st = file.readAll();

    if(st.isEmpty())
    {
        QMessageBox::critical(nullptr, "Critical", "Файл сохранения пуст. Восстановите его вручную");
        return 2;
    }

    file.close();

    QRegularExpression re("~[A-z]*~");
    QList data = st.split(re);
    misc = {data[0]};
    data.pop_front();

    for(QString& str : data)
    {
        QJsonParseError err;

        QString chunk = str;
        str = chunk.split(';')[0];
        misc.push_back(chunk.section(';', 1));

        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &err);

        if(doc.isNull())
        {
            qDebug() << "Error: " << err.errorString();
            QMessageBox::critical(nullptr, "Critical", "Ошибка парсинга. Возможно, файл сохранения поврежден. Восстановите его вручную");
            return 3;
        }
        json.push_back(doc.object());
    }

    if(!QFile::exists(appdata + path + "_bckp")) QFile::copy(appdata + path, appdata + path + "_bckp");
    return 0;
}

QList<QJsonObject>& JsonParser::getJson()
{
    return json;
}

int JsonParser::write()
{
    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", "Файл "  + appdata + path + " не был найден.");
        return 1;
    }

    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append | QIODevice::Truncate);

    file.write(misc[0].toUtf8());
    file.write("~SettingsSplit~");

    for(size_t i = 0; i < json.size(); ++i)
    {
        file.write(QJsonDocument(json[i]).toJson() + ";");
        file.write(misc[i+1].toUtf8());
        if(i != json.size() - 1) file.write("~NoticeMeSenpaiNoticeMe~");
    }

    return 0;
}

void JsonParser::clear()
{
    json.clear();
    misc.clear();
}
