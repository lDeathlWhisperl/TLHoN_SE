#ifndef TAB_SETTINGS_H
#define TAB_SETTINGS_H

#include <QWidget>
#include <QTranslator>

namespace Ui {
class Tab_Settings;
}

class Tab_Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Tab_Settings(QWidget *parent = nullptr);
    static int getCharacterId();
    static QString getLanguage();
    void initLanguage();
    ~Tab_Settings();

private:
    void    saveSettings(const QString &key, const QString &value);
    QString loadSettings(const QString &key);
    void loadGameSaves();

signals:
    void characterChanged();
    void modeToggled(bool);
    void saveRestored();
    void languageChanged();

private slots:
    void on_cb_saves_currentIndexChanged(int index);
    void on_btn_restore_clicked();
    void on_cb_cheater_toggled(bool checked);
    void on_le_maxIconUses_editingFinished();
    void on_cb_language_currentIndexChanged(int index);

private:
    Ui::Tab_Settings *ui;
    QTranslator translator;
    static QString language;
    static int characterId;
};

#endif // TAB_SETTINGS_H
