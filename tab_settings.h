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
    void modeToggled(bool);

private slots:
    void on_cb_saves_currentIndexChanged(int index);
    void on_btn_restore_clicked();

    void on_cb_cheater_toggled(bool checked);

private:
    Ui::Tab_Settings *ui;
};

#endif // TAB_SETTINGS_H
