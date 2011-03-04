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
#include "segmentation_options.hh"
#include "config.hh"

// Defines enum of binarization algorithms
# include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

using namespace scribo::toolchain::internal;

segmentation_options::segmentation_options(QWidget *parent)
  : OptionWidget(parent)
{
  setupUi(this);

  find_sepsCbox->insertItem(defs::Lines, "Lines");
  find_sepsCbox->insertItem(defs::Whitespaces, "Whitespaces");
  find_sepsCbox->insertItem(defs::LinesAndWhitespaces, "Lines and whitespaces");

  load_config();
}

segmentation_options::~segmentation_options()
{
}


void segmentation_options::load_config()
{
  config * const conf = config::get_instance();

  find_sepsCbox->setCurrentIndex(conf->segmentation_find_seps());
}


void segmentation_options::save_config()
{
  config * const conf = config::get_instance();

  conf->set_segmentation_find_seps(find_sepsCbox->currentIndex());
}
