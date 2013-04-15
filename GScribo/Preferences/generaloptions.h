#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H

#include <QFileDialog>

#include "optionwidget.h"
#include "configs.h"

namespace Ui
{
    class GeneralOptions;
}

class GeneralOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit GeneralOptions(QWidget *parent = 0);
        ~GeneralOptions();

        virtual void loadConfig();
        virtual void saveConfig();

    private:
        Ui::GeneralOptions *ui;

    private slots:
        void onCustomDirBrowseBtnClicked();

};

#endif // GENERAL_OPTIONS_H
