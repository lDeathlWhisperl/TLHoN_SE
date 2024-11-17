#include "mainwindow.h"

#include <QApplication>
#include "stylefromfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    setStyleFromFile(&w, ":/Resourses/StyleSheets/Global.qss");

    w.show();
    return a.exec();
}
