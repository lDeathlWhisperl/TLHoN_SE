#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcessEnvironment>

class JsonParser
{
    static QList<QJsonObject> json;
    static QList<QString> misc;
    static inline QString appdata = QProcessEnvironment::systemEnvironment().value("AppData");
    static const inline QString path = "LocalLow\\OverTheMoon\\TLHON\\SaveData.txt";

public:
    JsonParser() = delete;
    JsonParser(JsonParser&) = delete;
    JsonParser(JsonParser&&) = delete;

    static void init();

    static QList<QJsonObject>& getJson();

    static void write();

    static void clear();
};

#endif // JSONPARSER_H
