#include "progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent):
        QProgressDialog(parent, Qt::Dialog)
{
    setAutoClose(false);
    setCancelButton(0);
    hide();
}

void ProgressDialog::reset()
{
    setValue(0);
    setLabelText("Initilization");
    show();
}
