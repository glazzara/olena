#ifndef PROCESS_H
#define PROCESS_H

#include <scribo/toolchain/internal/content_in_doc_functor.hh>
#include <mln/core/image/image2d.hh>
#include <QObject>

using namespace scribo::toolchain::internal;

struct Process :
        public QObject,
        public content_in_doc_functor<mln::image2d<bool> >
{
        Q_OBJECT
        typedef content_in_doc_functor<mln::image2d<bool> > superT;

    public :
        explicit Process(const char *docFilename);
        virtual void on_progress();
        virtual void on_new_progress_label(const char *label);
        virtual void on_xml_saved();

    signals:
        void newProgressLabel(const QString& label);
        void progress();
        void xmlSaved(const QString& filename);
};

#endif // PROCESS_H
