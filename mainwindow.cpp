#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "stylesheet.h"

#include <QTabBar>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // setStyleSheet("");

    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);
    QFontDatabase::addApplicationFont(":/Resourses/Fonts/PressStart2P-Regular.ttf");

    //setStyle();
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
    connect(ui->btn_stats, &QPushButton::toggled,     [this]() { onTabButtonClicked(ui->btn_stats, ui->l_stats, 2); });
    connect(ui->btn_settings, &QPushButton::toggled,  [this]() { onTabButtonClicked(ui->btn_settings, ui->l_settings, 3); });
    connect(ui->btn_help, &QPushButton::toggled,      [this]() { onTabButtonClicked(ui->btn_help, ui->l_help, 4); });
}

void MainWindow::setStyle()
{
    using SS = StyleSheet;

    ui->btn_equipment->setStyleSheet(SS::getTabButton(Button::tab_equipment, Button::tab_equipment_h));
    ui->btn_inventory->setStyleSheet(SS::getTabButton(Button::tab_inventory, Button::tab_inventory_h));
    ui->btn_stats->setStyleSheet(SS::getTabButton(Button::tab_stats, Button::tab_stats_h));
    ui->btn_settings->setStyleSheet(SS::getTabButton(Button::tab_settings, Button::tab_settings_h));
    ui->btn_help->setStyleSheet(SS::getTabButton(Button::tab_help, Button::tab_help_h));
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
    checked_btn = button;
    checked_label = label;

    ui->tabWidget->setCurrentIndex(tabIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete checked_btn;
    delete checked_label;
}
