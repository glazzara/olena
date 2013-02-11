#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <QObject>

using namespace scribo::toolchain::internal;

class Preprocess :
        public QObject,
        public text_in_doc_preprocess_functor<mln::image2d<mln::value::rgb8> >
{
        Q_OBJECT

    public:
        virtual void on_progress();
        virtual void on_new_progress_label(const char *label);

    signals:
        void newProgressLabel(const QString& label);
        void progress();
};

#endif // PREPROCESS_H
