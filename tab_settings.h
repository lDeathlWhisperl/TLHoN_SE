#ifndef TAB_SETTINGS_H
#define TAB_SETTINGS_H

#include <QWidget>

namespace Ui {
class Tab_Settings;
}

class Tab_Settings : public QWidget
{
    Q_OBJECT

    static int characterId;
public:
    explicit Tab_Settings(QWidget *parent = nullptr);
    static int getCharacterId();
    ~Tab_Settings();

signals:
    void characterChanged();

private slots:
    void on_cb_saves_currentIndexChanged(int index);

private:
    Ui::Tab_Settings *ui;
};

#endif // TAB_SETTINGS_H
