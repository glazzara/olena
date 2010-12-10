// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#include "process.hh"

process::process(const char *doc_filename)
  : super_t(doc_filename)
{

}

void process::on_progress()
{
  emit progress();
}


void process::on_new_progress_label(const char *label)
{
  QString lbl(label);
  emit new_progress_label(label);
}


void process::on_xml_saved()
{
  QString filename(output_file.c_str());
  emit xml_saved(filename);
}
