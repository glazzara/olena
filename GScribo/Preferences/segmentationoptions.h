#ifndef SEGMENTATIONOPTIONS_H
#define SEGMENTATIONOPTIONS_H

//#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

#include "optionwidget.h"
#include "configs.h"
#include "region.h"

namespace Ui
{
    class SegmentationOptions;
}

class SegmentationOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit SegmentationOptions(QWidget *parent = 0);
        ~SegmentationOptions();

        void loadConfig();
        void saveConfig();

    private:
        Ui::SegmentationOptions *ui;
};

#endif // SEGMENTATIONOPTIONS_H
