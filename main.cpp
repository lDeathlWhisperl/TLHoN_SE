#include "mainwindow.h"
#include "stylefromfile.h"
#include "jsonparser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    JsonParser::init();
    QApplication a(argc, argv);
    MainWindow w;

    setStyleFromFile(&w, ":/Resourses/StyleSheets/Global.qss");

    w.show();
    return a.exec();
}
