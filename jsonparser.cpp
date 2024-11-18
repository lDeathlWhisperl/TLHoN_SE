#include "jsonparser.h"

#include <QFile>
#include <QRegularExpression>

QList<QJsonObject> JsonParser::json;
QList<QString> JsonParser::misc;

void JsonParser::init()
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString st = file.readAll();
    file.close();

    QRegularExpression re("~[A-z]*~");
    QList data = st.split(re);
    QList misc = {data[0]};
    data.pop_front();

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
}

QList<QJsonObject>& JsonParser::getJson()
{
    return json;
}

void JsonParser::write()
{
    QFile file;
    file.setFileName(path);
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
}
