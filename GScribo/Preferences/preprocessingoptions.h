#ifndef PREPROCESSINGOPTIONS_H
#define PREPROCESSINGOPTIONS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

#include "optionwidget.h"
#include "region.h"
#include "configs.h"

namespace Ui
{
    class PreprocessingOptions;
}

class PreprocessingOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit PreprocessingOptions(QWidget *parent = 0);
        ~PreprocessingOptions();

        void loadConfig();
        void saveConfig();

    private:
        Ui::PreprocessingOptions *ui;

};

#endif // PREPROCESSINGOPTIONS_H
