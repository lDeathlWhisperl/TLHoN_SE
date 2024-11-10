#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>

#define SICQ static inline const QString
struct Button final
{
private:
public:
    Button() = delete;
    Button(Button&) = delete;
    Button(Button&&) = delete;

    SICQ tab_equipment   = ":/Resourses/Images/Tabs/CategoryTab_Equipment.png";
    SICQ tab_equipment_h = ":/Resourses/Images/Tabs/CategoryTab_Equipment_Highlighted.png";

    SICQ tab_inventory   = ":/Resourses/Images/Tabs/CategoryTab_Inventory.png";
    SICQ tab_inventory_h = ":/Resourses/Images/Tabs/CategoryTab_Inventory_Highlighted.png";

    SICQ tab_stats   = ":/Resourses/Images/Tabs/Category_Tabs_PlayerStats.png";
    SICQ tab_stats_h = ":/Resourses/Images/Tabs/Category_Tabs_PlayerStats_Highlighted.png";

    SICQ tab_settings   = ":/Resourses/Images/Tabs/CategoryTab_Settings.png";
    SICQ tab_settings_h = ":/Resourses/Images/Tabs/CategoryTab_Settings_Highlighted.png";

    SICQ tab_help   = ":/Resourses/Images/Tabs/CategoryTab_Help.png";
    SICQ tab_help_h = ":/Resourses/Images/Tabs/CategoryTab_Help_Highlighted.png";
};
#undef SICQ

class StyleSheet final
{
public:
    StyleSheet() = delete;
    StyleSheet(StyleSheet&) = delete;
    StyleSheet(StyleSheet&&) = delete;

    static QString getTabButton(const QString& btn, const QString& btn_h);
};

#endif // STYLESHEET_H
