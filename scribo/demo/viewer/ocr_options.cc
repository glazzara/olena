// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#include "defs.hh"
#include "ocr_options.hh"
#include "config.hh"

// Defines enum of binarization algorithms
# include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

using namespace scribo::toolchain::internal;


static const char *language[][2] = {
  { "English", "eng" },
  { "French", "fra" },
  { 0, 0 }
};


ocr_options::ocr_options(QWidget *parent)
  : OptionWidget(parent)
{
  setupUi(this);

  for (unsigned i = 0; language[i][0]; ++i)
    ocr_language->insertItem(i, language[i][0]);

  load_config();
}

ocr_options::~ocr_options()
{
}


int ocr_options::find_index(const QString& lang)
{
  for (unsigned i = 0; language[i][0]; ++i)
    if (lang == language[i][1])
      return i;
  return 0;
}


void ocr_options::load_config()
{
  config * const conf = config::get_instance();

  enable_ocr->setChecked(conf->ocr_enabled());
  ocr_language->setCurrentIndex(find_index(conf->ocr_language()));
}


void ocr_options::save_config()
{
  config * const conf = config::get_instance();

  conf->set_ocr_enabled(enable_ocr->isChecked());
  conf->set_ocr_language(language[ocr_language->currentIndex()][1]);
}
