// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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
#include "preprocessing_options.hh"
#include "config.hh"

// Defines enum of binarization algorithms
# include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

using namespace scribo::toolchain::internal;

preprocessing_options::preprocessing_options(QWidget *parent)
  : OptionWidget(parent)
{
  setupUi(this);

  bin_algoCbox->insertItem(Convert, "Violent convert");
  bin_algoCbox->insertItem(Sauvola, "Local threshold");
  bin_algoCbox->insertItem(SauvolaMs,
			   "Local threshold multiscale");

  load_config();
}

preprocessing_options::~preprocessing_options()
{
}


void preprocessing_options::load_config()
{
  config * const conf = config::get_instance();

  subsampleCb->setChecked(conf->preprocessing_subsample());
  remove_bgCb->setChecked(conf->preprocessing_remove_bg());
  deskewCb->setChecked(conf->preprocessing_deskew());
  remove_noiseCb->setChecked(conf->preprocessing_remove_noise());
  bin_algoCbox->setCurrentIndex(conf->preprocessing_bin_algo());
}


void preprocessing_options::save_config()
{
  config * const conf = config::get_instance();

  conf->set_preprocessing_subsample(subsampleCb->isChecked());
  conf->set_preprocessing_remove_bg(remove_bgCb->isChecked());
  conf->set_preprocessing_deskew(deskewCb->isChecked());
  conf->set_preprocessing_remove_noise(remove_noiseCb->isChecked());
  conf->set_preprocessing_bin_algo(bin_algoCbox->currentIndex());
}
