#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

class JsonParser
{
    static QList<QJsonObject> json;
    static QList<QString> misc;
    static const inline QString path = "E:\\Projects\\Qt\\TLHoN_IC\\Resourses\\Tests\\SaveData.txt";
public:
    JsonParser() = delete;
    JsonParser(JsonParser&) = delete;
    JsonParser(JsonParser&&) = delete;

    static void init();

    static QList<QJsonObject>& getJson();

    static void write();
};

#endif // JSONPARSER_H
