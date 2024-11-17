#include "stylefromfile.h"

#include <QFile>
#include <QWidget>

void setStyleFromFile(QWidget *w, QString res)
{
    QFile file(res);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&file);
        w->setStyleSheet(ts.readAll());
        file.close();
    }
    else qWarning("Unable to open stylesheet file.");
}
