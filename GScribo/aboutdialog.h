#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_aboutdialog.h"

namespace Ui
{
    class AboutDialog;
}

class AboutDialog :
        public QDialog
{
    public:
        explicit AboutDialog(QWidget *parent = 0) : QDialog(parent, Qt::Dialog)
        { ui = new Ui::AboutDialog; ui->setupUi(this); }
        ~AboutDialog() { delete ui; }

    private:
        Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
