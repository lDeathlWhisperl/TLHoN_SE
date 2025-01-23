#include "jsonparser.h"

#include <QFile>
#include <QRegularExpression>
#include <QMessageBox>

QList<QJsonObject> JsonParser::json;
QList<QString> JsonParser::misc;

void JsonParser::init()
{
    appdata = appdata.split("Roaming")[0];

    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", "File "  + appdata + path + " not found.");
        qFatal() << "file not found";
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString st = file.readAll();

    if(st.isEmpty())
    {
        QMessageBox::critical(nullptr, "Critical", "File is empty. Recover it manually");
        qFatal() << "File is empty";
    }

    file.close();

    QRegularExpression re("~[A-z]*~");
    QList data = st.split(re);
    misc = {data[0]};
    data.pop_front();

    try
    {
        for(QString& str : data)
        {
            QJsonParseError err;

            QString chunk = str;
            str = chunk.split(';')[0];
            misc.push_back(chunk.section(';', 1));

            QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &err);

            if(doc.isNull())
            {
                qDebug() << " QJsonDocument error: " << err.errorString();
                QMessageBox::critical(nullptr, "Critical", "Parsing error. The save file may be corrupted. Please recover it manually.");
            }
            json.push_back(doc.object());
        }
    }
    catch (std::exception& e)
    {
        qFatal() << e.what();
        QMessageBox::critical(nullptr, "Critical", "Something went wrong :(");
    }

    if(!QFile::exists(appdata + path + "_bckp")) QFile::copy(appdata + path, appdata + path + "_bckp");
}

QList<QJsonObject>& JsonParser::getJson()
{
    return json;
}

void JsonParser::write()
{
    QFile file(appdata + path);

    if(!file.exists())
    {
        QMessageBox::critical(nullptr, "Critical", "File "  + appdata + path + " not found.");
        qFatal() << "file not found";
    }

    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append | QIODevice::Truncate);

    file.write(misc[0].toUtf8());
    file.write("~SettingsSplit~");

    try
    {
        for(size_t i = 0; i < json.size(); ++i)
        {
            file.write(QJsonDocument(json[i]).toJson() + ";");
            file.write(misc[i+1].toUtf8());
            if(i != json.size() - 1) file.write("~NoticeMeSenpaiNoticeMe~");
        }
    }
    catch (std::exception& e)
    {
        qFatal() << e.what();
        QMessageBox::critical(nullptr, "Critical", "Something went wrong :(");
    }
}

void JsonParser::clear()
{
    json.clear();
    misc.clear();
}
