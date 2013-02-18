#include "process.h"

Process::Process(const char *doc_filename)
  : superT(doc_filename)
{
}

void Process::on_progress()
{
    emit progress();
}

void Process::on_new_progress_label(const char *label)
{
    emit newProgressLabel(QString(label));
}

void Process::on_xml_saved()
{
    emit xmlSaved(QString(output_file.c_str()));
}
