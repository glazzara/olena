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

#include "general_options.hh"
#include "config.hh"


general_options::general_options(QWidget *parent)
  : OptionWidget(parent)
{
  setupUi(this);

  load_config();
}

general_options::~general_options()
{
}


void general_options::load_config()
{
  config * const conf = config::get_instance();

  saveXml->setChecked(conf->general_save_xml_enabled());
  sameDir->setChecked(conf->general_save_xml_same_dir());
  customDir->setChecked(conf->general_save_xml_custom_dir());
  customDirValue->setText(conf->general_save_xml_custom_dir_path());
}


void general_options::save_config()
{
  config * const conf = config::get_instance();

  conf->set_general_save_xml_enabled(saveXml->isChecked());
  conf->set_general_save_xml_same_dir(sameDir->isChecked());
  conf->set_general_save_xml_custom_dir(customDir->isChecked());
  conf->set_general_save_xml_custom_dir_path(customDirValue->text());
}

void general_options::on_customDirBrowseBtn_clicked()
{
  QString dir = QFileDialog::getExistingDirectory(0, "Choose a directory");
  if (!dir.isEmpty())
    customDirValue->setText(dir);
}
