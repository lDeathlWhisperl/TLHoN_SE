#ifndef TAB_STATS_H
#define TAB_STATS_H

#include <QWidget>

namespace Ui {
class Tab_stats;
}

class Tab_Stats : public QWidget
{
    Q_OBJECT

    int id = 0;
public:
    explicit Tab_Stats(QWidget *parent = nullptr);
    void initSetting(class Tab_Settings* s);
    ~Tab_Stats();

private slots:
    void update();

private:
    Ui::Tab_stats *ui;
};

#endif // TAB_STATS_H
