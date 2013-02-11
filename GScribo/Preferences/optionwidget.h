#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include <QWidget>

struct OptionWidget :
        public QWidget
{
    explicit OptionWidget(QWidget * parent) : QWidget(parent) {}

    virtual void loadConfig() {}
    virtual void saveConfig() {}
};

#endif // OPTIONWIDGET_H
