#include "mainwindow.h"
#include "stylefromfile.h"
#include "jsonparser.h"

#include <QApplication>
#include <QFile>
#include <QDateTime>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logMessage = QString("%1:%2:%3 - %4").arg(context.file).arg(context.line).arg(context.function).arg(msg);
    QString logFilePath = "log.txt";

    QFile file(logFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&file);
        ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - " << logMessage << Qt::endl;
    }

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", logMessage.toLocal8Bit().constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", logMessage.toLocal8Bit().constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", logMessage.toLocal8Bit().constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", logMessage.toLocal8Bit().constData());
        abort();
    case QtInfoMsg:
        break;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    qInfo() << "Started new session";
    QApplication a(argc, argv);

    JsonParser::init();
    MainWindow w;

    setStyleFromFile(&w, ":/Resources/StyleSheets/Global.qss");

    w.show();

    return a.exec();
}
