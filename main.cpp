#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("");

    QFile file(":/Resourses/StyleSheet.qss");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&file);
        w.setStyleSheet(ts.readAll());
        file.close();
    }
    else qWarning("Unable to open stylesheet file.");

    w.show();
    return a.exec();
}
