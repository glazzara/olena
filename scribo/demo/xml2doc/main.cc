// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
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

#include <iostream>
#include <string>
#include <string>
#include <cstdlib>
#include <Magick++.h>

#undef MLN_WO_GLOBAL_VARS
#include <mln/core/image/image2d.hh>
#include <mln/trace/all.hh>

#include "xml_transform.hh"

#include "datarootdir.hh"


void check_xsltproc()
{
  if (system("which xsltproc > /dev/null") == -1)
  {
    qDebug() << "FATAL ERROR: Conversion to HTML requires xsltproc to "
      "be installed!";
    exit(1);
  }
}


QString get_datarootdir(const QString& file)
{
  QFile f(SCRIBO_LOCAL_DATAROOTDIR "/templates/" + file);
  if (f.exists())
    return SCRIBO_LOCAL_DATAROOTDIR "/templates/";

  f.setFileName(SCRIBO_PREFIX_DATAROOTDIR "/templates/" + file);
  if (f.exists())
    return SCRIBO_PREFIX_DATAROOTDIR "/templates/";

  qDebug() << "FATAL ERROR: Can't locate file: " + file;
  exit (1);

  return "";
}



int html_base64(const QString& xml, const QString& html)
{
  qDebug() << QString("HTML output : %1").arg(html);
  QString datarootdir = get_datarootdir("html/main_base64.xsl");
  return system(QString("xsltproc \"%1/html/main_base64.xsl\" \"%2\" > %3")
		.arg(datarootdir).arg(xml).arg(html).toAscii().constData());
}


int pdf_base64(const QString& xml, const QString& pdf)
{
  qDebug() << QString("PDF output : %1").arg(pdf);
  QString datarootdir = get_datarootdir("pdf/main.xsl");
  return system(QString("fop -xsl %1/pdf/main.xsl -xml %2 -pdf %3")
		.arg(datarootdir).arg(xml).arg(pdf).toAscii().constData());
}


int svg_base64(const QString& xml, const QString& svg)
{
  qDebug() << QString("SVG output : %1").arg(svg);
  QString datarootdir = get_datarootdir("svg/main.xsl");
  return system(QString("fop -xsl %1/svg/main.xsl -xml %2 -svg %3")
		.arg(datarootdir).arg(xml).arg(svg).toAscii().constData());
}




int main(int argc, char **argv)
{
  QString man = \
    "xml_transform\n"
    "OPTIONS:\n\n"

    "HTML output:\n"
    "\t\033[01m--html\033[00m\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.html\033[00m"
    "\t\t\tCreates HTML file, images are embedded inside in base 64 format.\n"
    "\t\033[01m--html-base64\033[00m \033[04minput.xml\033[00m \033[04moutput.html\033[00m"
    "\t\t\tSame as --html but input is a XML file with images encoded in base 64 inside.\n"

    "\t\033[01m--html-full\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput_dir\033[00m"
    "\t\t\tCreates HTML file without converting images in base 64. Images + HTML file are placed in output_dir.\n\n"

    "PDF output:\n"
    "\t\033[01m--pdf\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.pdf\033[00m"
    "\t\t\tCreates PDF file, images are embedded inside in base 64 format.\n"
    "\t\033[01m--pdf-base64\033[00m \033[04minput.xml\033[00m \033[04moutput.pdf\033[00m"
    "\t\t\tSame as --pdf but input is a XML file with images encoded in base 64 inside.\n\n"

    "SVG output (experimental, may not work with all files):\n"
    "\t\033[01m--svg\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.svg\033[00m"
    "\t\t\tCreates SVG file, images are embedded inside in base 64 format.\n"
    "\t\033[01m--svg-base64\033[00m \033[04minput.xml\033[00m \033[04moutput.svg\033[00m"
    "\t\t\tSame as --svg but input is a XML file with images encoded in base 64 inside.\n\n"

    "OpenDocument output:\n"
    "\t\033[01m--oo-doc\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.odt\033[00m"
    "\t\t\tCreates Open Document Writer (odt) file.\n"
    "\tThis is experimental, Open Office will warn you and ask you to repair the file which is corrupted, click on repair.\n\n"

    "Base 64 operations:\n"
    "\t\033[01m--to-base64\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.xmlc\033[00m"
    "\t\t\tCreates a container XML file. It will contain regions which are cropped and converted in base 64.\n"

    "\t\033[01m--to-base64-no-crop\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.xmlc\033[00m"
    "\t\tSame as --to-base64 but only the full picture will be converted, regions are not cropped\n"

    "\t\033[01m--from-base64\033[00m \033[04minput.xml\033[00m \033[04moutput_dir\033[00m"
    "\t\t\tDecodes a XML file which has been encoded in base 64, regions will be converted into png files and saved in output_dir.\n\n"

    "Requirements:\n"
    "PDF : fop >= 0.95\n"
    "HTML : xsltproc\n"
    "SVG : trunk version of fop\n"
    "OpenDocument : ooconvert (included) and OpenOffice >= 3.xx";

  QString option(argv[1]);


  if (argc > 4)
  {
    if ("--html-full" == option)
    {
      check_xsltproc();

      XmlTransform xmlt(argv[2], argv[3], argv[4]);
      xmlt.createHTML(false);


      QString output = xmlt.out() + "output.html";
      qDebug() << QString("HTML output : %1").arg(output);
      QString datarootdir = get_datarootdir("html/main.xsl");
      int ret = system(QString("xsltproc \"%1/html/main.xsl\" \"%2/output.xml\" > %3")
		       .arg(datarootdir)
		       .arg(xmlt.out())
		       .arg(output).toAscii().constData());

      QFile::remove(xmlt.out() + "output.xml");
      return ret;
    }
    else if ("--html" == option)
    {
      QTemporaryFile tmp;
      tmp.open();
      tmp.fileName();

      XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
      xmlt.toBase64(false);

      return html_base64(tmp.fileName(), argv[4]);
    }
    else if ("--pdf" == option)
    {
      QTemporaryFile tmp;
      tmp.open();
      tmp.fileName();

      XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
      xmlt.toBase64(false);

      return pdf_base64(tmp.fileName(), argv[4]);
    }
    else if ("--svg" == option)
    {
      QTemporaryFile tmp;
      tmp.open();
      tmp.fileName();

      XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
      xmlt.toBase64(false);

      return svg_base64(tmp.fileName(), argv[4]);
    }
    else if ("--oo-doc" == option)
    {
      QString dir = QDir::tempPath() + "/xml_transform." + argv[4];
      XmlTransform xmlt(argv[2], argv[3], dir);
      xmlt.createOpen();

      QString datarootdir = get_datarootdir("html/main.xsl");
      system(QString("xsltproc \"%1/html/main.xsl\" \"%2/output.xml\" > %2/out.html")
	     .arg(datarootdir).arg(dir).toAscii().constData());

      qDebug() << QString("Open Document output : %1").arg(argv[4]);

      datarootdir = get_datarootdir("bin/ooconvert");
      system(QString(" \"%1/bin/ooconvert\" \"%2/out.html\" \"%2/out.odt\" > /dev/null")
	     .arg(datarootdir).arg(dir).toAscii().constData());

      QString cur_dir = QDir::currentPath();
      QString tmp_dir = dir + "/oo_tmp";

      QDir d;
      d.mkdir(tmp_dir);
      qDebug() << "unzip %1/out.odt -d %2 > /dev/null" << dir << tmp_dir;
      system(QString("unzip %1/out.odt -d %2 > /dev/null")
	     .arg(dir).arg(tmp_dir).toAscii().constData());

      system(QString("cat %1/content.xml | sed -re 's!\\.\\./([a-zA-Z0-9])!Pictures/\\1!g' "
		     ">> %1/tmp.xml").arg(tmp_dir).toAscii().constData());

      d.mkdir(tmp_dir + "/Pictures");
      d.rename(dir + "/*.png",  tmp_dir + "/Pictures/");
      d.rename(tmp_dir + "/tmp.xml", tmp_dir + "/content.xml");

      d.cd(tmp_dir);
      qDebug() << "zip zip.odt -r * > /dev/null";
      system("zip zip.odt -r * > /dev/null");

      QFile::copy("zip.odt", argv[4]);
      d.cd(cur_dir);

      d.rmdir(dir);

      return 0;
    }
    else if ("--to-base64" == option)
    {
      XmlTransform xmlt(argv[2], argv[3], QString::Null(), argv[4]);
      xmlt.toBase64(false);
      return 0;
    }
    else if ("--to-base64-no-crop" == option)
    {
      XmlTransform xmlt(argv[2], argv[3], QString::Null(), argv[4]);
      xmlt.toBase64(true);
      return 0;
    }
  }
  else if (argc > 3)
  {
    if ("--svg-base64" == option)
    {
      return svg_base64(argv[2], argv[3]);
    }
    else if ("--pdf-base64" == option)
    {
      return pdf_base64(argv[2], argv[3]);
    }
    else if ("--html-base64" == option)
    {
      return html_base64(argv[2], argv[3]);
    }
    else if ("--from-base64" == option)
    {
      XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
      xmlt.fromBase64();
      return 0;
    }
  }

  qDebug() << man;

  return 0;
}
