#include "mainwindow.h"
#include "stylefromfile.h"
#include "jsonparser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(JsonParser::init() != 0) return 0;
    MainWindow w;

    setStyleFromFile(&w, ":/Resources/StyleSheets/Global.qss");

    w.show();

    return a.exec();
}
