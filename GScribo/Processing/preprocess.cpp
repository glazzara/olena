#include "preprocess.h"

void Preprocess::on_progress()
{
    emit progress();
}

void Preprocess::on_new_progress_label(const char *label)
{
    emit newProgressLabel(QString(label));
}
