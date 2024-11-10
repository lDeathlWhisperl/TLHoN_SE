#include "stylesheet.h"


QString StyleSheet::getTabButton(const QString &btn, const QString &btn_h)
{
    return  "QPushButton"
            "{"
            "    background-image: url(" + btn + ");"
            "    background-repeat: no-repeat;"
            "    background-position: center;"
            "    border: none;"
            "}"
            "QPushButton::hover   { background-image: url(" + btn_h + "); }"
            "QPushButton::checked { background-image: url(" + btn_h + "); }";
}
