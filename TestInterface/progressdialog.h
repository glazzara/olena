#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QProgressDialog>

class ProgressDialog :
        public QProgressDialog
{
        Q_OBJECT

    public:
        explicit ProgressDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        void reset();

    public slots:
        void run();
};

#endif // PROGRESSDIALOG_H
