#ifndef TAB_EQUIPMENT_H
#define TAB_EQUIPMENT_H

#include <QWidget>

namespace Ui {
class Tab_Equipment;
}

class Tab_Equipment : public QWidget
{
    Q_OBJECT

public:
    explicit Tab_Equipment(QWidget *parent = nullptr);
    void initSettings(class Tab_Settings *s);
    ~Tab_Equipment();

private:
    Ui::Tab_Equipment *ui;
};

#endif // TAB_EQUIPMENT_H
