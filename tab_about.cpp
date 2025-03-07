#include "tab_about.h"
#include "ui_tab_about.h"

#include "stylefromfile.h"
#include "tab_settings.h"

#include<QDesktopServices>
#include <QUrl>

Tab_About::Tab_About(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab_About)
{
    ui->setupUi(this);
    setStyleFromFile(this, ":/Resources/StyleSheets/Tab_About.qss");

    connect(ui->btn_github, &QPushButton::clicked, []()
            {
                QUrl url("https://github.com/lDeathlWhisperl");
                QDesktopServices::openUrl(url);
            });
    connect(ui->btn_steam, &QPushButton::clicked, []()
            {
                QUrl url("https://steamcommunity.com/profiles/76561198111573610/");
                QDesktopServices::openUrl(url);
            });
    connect(ui->btn_report, &QPushButton::clicked, []()
            {
                QUrl url("https://github.com/lDeathlWhisperl/TLHoN_IC/issues");
                QDesktopServices::openUrl(url);
            });
    connect(ui->btn_guide, &QPushButton::clicked, []()
            {
                QUrl url(QObject::tr("https://steamcommunity.com/sharedfiles/filedetails/?id=3421728450"));
                QDesktopServices::openUrl(url);
            });
}

Tab_About::~Tab_About()
{
    delete ui;
}

void Tab_About::initSettings(Tab_Settings *s)
{
    connect(s, &Tab_Settings::languageChanged, [this](){ ui->retranslateUi(this); });
}
