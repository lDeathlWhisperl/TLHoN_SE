#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTabButtonClicked(class QPushButton *button, class QLabel* label, int tabIndex);

private:
    Ui::MainWindow *ui;
    class QPushButton *checked_btn;
    class QLabel *checked_label;

    // class Tab_Equipment *tab_equipment;
    class Tab_Stats *tab_stats;
    class Tab_Settings *tab_settings;
    class Tab_About *tab_about;
};

#endif // MAINWINDOW_H
