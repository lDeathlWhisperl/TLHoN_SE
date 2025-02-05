#ifndef TAB_ABOUT_H
#define TAB_ABOUT_H

#include <QWidget>

namespace Ui {
class Tab_About;
}

class Tab_About : public QWidget
{
    Q_OBJECT

public:
    explicit Tab_About(QWidget *parent = nullptr);
    ~Tab_About();

    void initSettings(class Tab_Settings* s);

private:
    Ui::Tab_About *ui;
};

#endif // TAB_ABOUT_H
