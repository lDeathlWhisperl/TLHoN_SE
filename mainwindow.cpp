#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "tab_equipment.h"
#include "tab_stats.h"
#include "tab_settings.h"

#include <QTabBar>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);
    QFontDatabase::addApplicationFont(":/Resources/Fonts/PressStart2P-Regular.ttf");
    setWindowIcon(QIcon(":/Resources/Images/Misc/icon.ico"));

    tab_equipment = new Tab_Equipment(ui->Equipment);
    tab_stats = new Tab_Stats(ui->Stats);
    tab_settings = new Tab_Settings(ui->Settings);

    tab_equipment->initSettings(tab_settings);
    tab_stats->initSettings(tab_settings);

    ui->btn_equipment->setCheckable(true);
    ui->btn_inventory->setCheckable(true);
    ui->btn_stats->setCheckable(true);
    ui->btn_settings->setCheckable(true);
    ui->btn_help->setCheckable(true);

    ui->l_equipment->setVisible(true);
    ui->l_inventory->setVisible(false);
    ui->l_stats->setVisible(false);
    ui->l_settings->setVisible(false);
    ui->l_help->setVisible(false);

    checked_btn = ui->btn_equipment;
    checked_label = ui->l_equipment;
    ui->btn_equipment->setChecked(true);

    connect(ui->btn_equipment, &QPushButton::toggled, [this]() { onTabButtonClicked(ui->btn_equipment, ui->l_equipment, 0); });
    connect(ui->btn_inventory, &QPushButton::toggled, [this]() { onTabButtonClicked(ui->btn_inventory, ui->l_inventory, 1); });
    connect(ui->btn_stats,     &QPushButton::toggled, [this]() { onTabButtonClicked(ui->btn_stats,     ui->l_stats,     2); });
    connect(ui->btn_settings,  &QPushButton::toggled, [this]() { onTabButtonClicked(ui->btn_settings,  ui->l_settings,  3); });
    connect(ui->btn_help,      &QPushButton::toggled, [this]() { onTabButtonClicked(ui->btn_help,      ui->l_help,      4); });

    connect(tab_settings, &Tab_Settings::languageChanged, [this]() { emit ui->retranslateUi(this); });

    ui->vl_equipment_layout->addWidget(tab_equipment);
    ui->vl_stats_layout->addWidget(tab_stats);
    ui->vl_settings_layout->addWidget(tab_settings);

    tab_equipment->show();
    tab_stats->show();
    tab_settings->show();

    tab_settings->initLanguage();
}

void MainWindow::onTabButtonClicked(QPushButton *button, QLabel *label, int tabIndex)
{
    if(checked_btn != button)
    {
        checked_btn->setEnabled(true);
        button->setEnabled(false);
    }

    checked_btn->setChecked(false);
    checked_label->setVisible(false);

    label->setVisible(true);
    checked_btn  = button;
    checked_label = label;

    ui->tabWidget->setCurrentIndex(tabIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete checked_btn;
    delete checked_label;

    delete tab_equipment;
    delete tab_stats;
    delete tab_settings;
}

