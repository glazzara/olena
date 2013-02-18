#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QProgressDialog>

class ProgressDialog :
        public QProgressDialog
{
        Q_OBJECT

    public:
        explicit ProgressDialog(QWidget *parent = 0);
        void reset();

    public slots:
        inline void run();
};

void ProgressDialog::run()
{ setValue(value()+1); }

#endif // PROGRESSDIALOG_H
