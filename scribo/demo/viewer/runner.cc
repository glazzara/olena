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

#include "runner.hh"

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>

#include "demodir.hh"

#include "process.hh"
#include "preprocess.hh"
#include "config.hh"
#include "defs.hh"



using namespace mln;
using namespace scribo::toolchain::internal;

static
QString get_pathto(const QString& file,
		   const QString localdirsuffix = QString())
{
  QFile f(SCRIBO_LOCAL_DEMODIR "/" + localdirsuffix + "/" + file);
  if (f.exists())
    return SCRIBO_LOCAL_DEMODIR "/" + localdirsuffix;

  f.setFileName(SCRIBO_PREFIX_BINDIR "/" + file);
  if (f.exists())
    return SCRIBO_PREFIX_BINDIR;

  qDebug() << "FATAL ERROR: Can't locate file: " + file;

  return "";
}


runner::runner(QObject *parent)
  : QThread(parent)
{
  moveToThread(this);
}


void runner::run()
{

  switch(mode_)
  {
    default:
    case Demat:
    {
      image2d<value::rgb8> ima;
      io::magick::load(ima, args_.at(0).toUtf8().constData());

      image2d<bool> bin_ima = preprocess(ima);
      process(ima, bin_ima);
    }
    break;

    case Export:
      export_as();
    break;
  }

  emit finished();
  qDebug() << "Done.";
}


void runner::stop()
{
  terminate();
}



// Demat related stuff

void runner::start_demat(const QString& filename)
{
  args_.clear();
  args_ << filename;
  mode_ = Demat;

  QThread::start();
}


image2d<bool>
runner::preprocess(const image2d<value::rgb8>& ima)
{
  emit new_step("Preprocessing");

  ::preprocess f;

  // Load config.
  config * const conf = config::get_instance();

  connect(&f, SIGNAL(progress()), this, SIGNAL(progress()));
  connect(&f, SIGNAL(new_progress_label(const QString&)),
	  this, SIGNAL(new_progress_label(const QString&)));

  f.enable_subsample = conf->preprocessing_subsample();
  f.enable_fg_extraction = conf->preprocessing_remove_bg();
  f.enable_deskew = conf->preprocessing_deskew();
  f.enable_denoising = conf->preprocessing_remove_noise();

  f.binarization_algo = static_cast<Binarization_Algo>(conf->preprocessing_bin_algo());

  emit new_progress_max_value(f.nsteps());

  // Perform preprocessing.
  f(ima);

  qDebug() << "Preprocess Done.";
  return f.output;
}


void runner::process(const image2d<value::rgb8>& original_ima,
		     const image2d<bool>& processed_ima)
{
  emit new_step("Page segmentation");

  ::process f(args_.at(0).toUtf8().constData());

  connect(&f, SIGNAL(progress()), this, SIGNAL(progress()));
  connect(&f, SIGNAL(new_progress_label(const QString&)),
	  this, SIGNAL(new_progress_label(const QString&)));
  connect(&f, SIGNAL(xml_saved(const QString&)),
	  this, SIGNAL(xml_saved(const QString&)));

  // Load config.
  config * const conf = config::get_instance();

  defs::FindSeparators
    find_seps = static_cast<defs::FindSeparators>(conf->segmentation_find_seps());
  f.enable_line_seps = (find_seps == defs::Lines
			|| find_seps == defs::LinesAndWhitespaces);
  f.enable_whitespace_seps = (find_seps == defs::Whitespaces
			      || find_seps == defs::LinesAndWhitespaces);
  f.enable_ocr = conf->ocr_enabled();
  f.ocr_language = conf->ocr_language().toAscii().data();
  f.xml_format = scribo::io::xml::PageExtended;


  f.save_doc_as_xml = true;
  QFileInfo file(args_.at(0));
  QString output_dir = QDir::tempPath();
  if (conf->general_save_xml_enabled())
  {
    if (conf->general_save_xml_same_dir())
      output_dir = file.absolutePath();
    else if (conf->general_save_xml_custom_dir())
      output_dir = conf->general_save_xml_custom_dir_path();
    else
      qDebug() << "runner::progress - Invalid xml saving option!";

    QDir dir(output_dir);
    if (!dir.exists() && !dir.mkpath(output_dir))
      output_dir = QDir::tempPath();
  }
  f.output_file = (output_dir  + "/" + file.baseName() + "_gui.xml").toUtf8().constData();

  emit new_progress_max_value(f.nsteps());

  // Perform text detection.
  f(original_ima, processed_ima);

  qDebug() << "Process Done.";
}



// Export related stuff

void runner::start_export(const QString& imgfile,
			  const QString& xmlfile, const QString& outfile)
{
  args_.clear();
  args_ << imgfile << xmlfile << outfile;
  mode_ = Export;

  QThread::start();
}


void runner::export_as()
{
  emit new_step("Exporting document...");
  emit new_progress_max_value(2);

  // Checking output format
  QFileInfo f(args_.at(2));

  QString pathto_xml2doc = get_pathto("scribo-xml2doc", "xml2doc");

  if (pathto_xml2doc.isEmpty())
  {
    QMessageBox::critical(0, "Fatal error", "Cannot export! Cannot find scribo-xml2doc program!");
    return;
  }

  emit progress();

  int rvalue = 0;
  if (f.suffix() == "pdf")
  {
    emit new_step("Exporting as PDF...");
    rvalue = system(QString("%1/scribo-xml2doc --pdf %2 %3 %4")
		    .arg(pathto_xml2doc).arg(args_.at(1)).arg(args_.at(0))
		    .arg(args_.at(2)).toAscii().constData());
  }
  else if (f.suffix() == "html" || f.suffix() == "htm")
  {
    emit new_step("Exporting as HTML...");
    rvalue = system(QString("%1/scribo-xml2doc --html %2 %3 %4")
		    .arg(pathto_xml2doc).arg(args_.at(1)).arg(args_.at(0))
		    .arg(args_.at(2)).toAscii().constData());
  }
  else
    QMessageBox::critical(0, "Fatal error", "Cannot export! Invalid output format!");

  if (rvalue != 0)
    QMessageBox::critical(0, "Fatal error", "Cannot export! Return value is not 0!");
}
