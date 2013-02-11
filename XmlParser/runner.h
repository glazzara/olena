#ifndef RUNNER_H
#define RUNNER_H

#include <QThread>
#include <QStringList>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>

enum RunMode
{
    Demat,
    Export
};

class Runner :
        public QThread
{
        //Q_OBJECT

    public:
        explicit Runner(QObject *parent = 0);
};

#endif // RUNNER_H
