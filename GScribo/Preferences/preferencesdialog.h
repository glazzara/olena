#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "preprocessingoptions.h"
#include "segmentationoptions.h"
#include "generaloptions.h"
#include "ocroptions.h"

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog :
        public QDialog
{
        Q_OBJECT

    public:
        explicit PreferencesDialog(QWidget *parent = 0);
        ~PreferencesDialog();

    private:
        void loadOptionList();
        void selectOptionWidget(int row);

        Ui::PreferencesDialog *ui;
        QVector<QWidget *> widgets;

    private slots:
        inline void onOptionListCurrentRowChanged(int row);
        virtual void accept();
        virtual void reject();

};

inline void PreferencesDialog::onOptionListCurrentRowChanged(int row)
{ selectOptionWidget(row); }

#endif // PREFERENCESDIALOG_H
