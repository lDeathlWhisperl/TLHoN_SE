#ifndef TAB_STATS_H
#define TAB_STATS_H

#include <QWidget>

namespace Ui {
class Tab_stats;
}

class Tab_Stats : public QWidget
{
    Q_OBJECT

public:
    explicit Tab_Stats(QWidget *parent = nullptr);
    ~Tab_Stats();

private:
    Ui::Tab_stats *ui;
};

#endif // TAB_STATS_H
