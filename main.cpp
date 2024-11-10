#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":/Resourses/StyleSheet.qss"); // Укажите путь к вашему QSS файлу
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&file);
        w.setStyleSheet(ts.readAll()); // Применяем стили к приложению
        file.close();
    }
    else qWarning("Unable to open stylesheet file.");

    w.show();
    return a.exec();
}
