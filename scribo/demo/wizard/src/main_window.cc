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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <src/main_window.hh>

#include <src/load_page.hh>
#include <src/doc_type_page.hh>
#include <src/crop_page.hh>
#include <src/preprocessing_page.hh>
#include <src/process_page.hh>
#include <src/rotate_page.hh>
#include <src/result_page.hh>

namespace scribo
{

  namespace demo
  {

    namespace wizard
    {


      main_window::main_window()
      {
	QPixmap bg_pixmap(":/images/images/dematerialization.png");
	setPixmap(QWizard::WatermarkPixmap, bg_pixmap);
	setPixmap(QWizard::BackgroundPixmap, bg_pixmap);


	connect(this, SIGNAL(currentIdChanged(int)),
		this, SLOT(showCustomButton_slot(int)));


	// Page_Load
	load_page* page_load = new load_page(this);
	addPage(page_load);

	// Page_Task
	doc_type_page* page_doc_type = new doc_type_page(this);
	addPage(page_doc_type);

	// Page_Crop
	crop_page* page_crop = new crop_page(this);
	addPage(page_crop);

	// Page_Crop
	rotate_page* page_rotate = new rotate_page(this);
	addPage(page_rotate);

	// Page_Preprocessing
 	preprocessing_page*
	  page_preprocessing = new preprocessing_page(this);
 	addPage(page_preprocessing);

	// Page_Processing
	addPage(new process_page(this));

	// Page_Result
	addPage(new result_page(this));
      }


      int main_window::nextId() const
      {
	switch (currentId())
	{
	  case Page_Load:
	    if (field("load.must_crop").toBool())
	      return Page_Crop;
	    else if (field("load.must_rotate").toBool())
	      return Page_Rotate;
	    else
	      return Page_Task;

	  case Page_Crop:
	    if (field("load.must_rotate").toBool())
	      return Page_Rotate;
	    else
	      return Page_Task;

	  case Page_Rotate:
	    return Page_Task;

	  case Page_Task:
	    return Page_Preprocessing;

	  case Page_Preprocessing:
	    return Page_Process;

	  case Page_Process:
	    return Page_Result;

	  case Page_Result:
	    return -1;

	  default:
	    qDebug() << "Warning: using default nextId() case.";
	    return currentId() + 1;
	}
      }


      void main_window::showCustomButton_slot(int id)
      {
	if (id == Page_Result)
	  setOptions(options() | QWizard::HaveCustomButton1);
	else
	{
	  setOptions(QWizard::NoBackButtonOnStartPage
//    		     | QWizard::DisabledBackButtonOnLastPage
		     | QWizard::NoCancelButton);
	}
      }

    } // end of namespace scribo::demo::wizard

  } // end of namespace scribo::demo

} // end of namespace scribo
