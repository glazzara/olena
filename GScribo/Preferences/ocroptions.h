#ifndef OCR_OPTIONS_H
#define OCR_OPTIONS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

#include "optionwidget.h"
#include "configs.h"
#include "region.h"

namespace Ui
{
    class OcrOptions;
}

class OcrOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit OcrOptions(QWidget *parent = 0);
        ~OcrOptions();

        void loadConfig();
        void saveConfig();

    private:
        int findIndex(const QString& lang);

        Ui::OcrOptions *ui;
};

#endif // OCR_OPTIONS_H
