#include "tab_stats.h"
#include "ui_tab_stats.h"
#include "stylefromfile.h"

#include <QFile>

Tab_Stats::Tab_Stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_stats)
{
    ui->setupUi(this);

    setStyleFromFile(this, ":/Resourses/StyleSheets/Tab_Stats.qss");
}

Tab_Stats::~Tab_Stats()
{
    delete ui;
}
