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

# include "loader.hh"
# include "dommodel.hh"
# include "xml_transform.hh"

Loader::Loader()
{
}

Loader::~Loader()
{
}

DomModel* Loader::xml_to_dom(QString output)
{
  QString file_path = output;

  QFile file(file_path);

  if (file.open(QIODevice::ReadOnly))
    {
      QDomDocument document;
      if (document.setContent(&file))
	{
	  return new DomModel(document);
	}
    }

  return 0;
}

bool Loader::set_output(QString& output)
{
  QDir dir(output);

  if (!output.endsWith("/"))
    output.append("/");

  if (!dir.exists())
    {
      return dir.mkpath(".");
    }
  else
    {
      QStringList list = dir.entryList(QDir::Writable | QDir::AllDirs);
      return list.size() != 0;
    }
}


void Loader::add_html_templates(QString output)
{

  QFile gen("templates/html/html_generator.sh");
  gen.copy(output + "html_generator.sh");

  QFile css("templates/html/css.css");
  css.copy(output + "css.css");

  QFile xsl("templates/html/xsl.xsl");
  xsl.copy(output + "xsl.xsl");

}

void Loader::add_pdf_templates(bool crop, QString output)
{

  QFile regions("templates/pdf/regions.xsl");
  regions.copy(output + "regions.xsl");

  QFile gen("templates/pdf/pdf_generator.sh");
  gen.copy(output + "pdf_generator.sh");

  QFile line("templates/pdf/line.xsl");
  line.copy(output + "line.xsl");
  if (crop)
    {
      QFile xsl("templates/pdf/main_crop.xsl");
      xsl.copy(output + "main.xsl");
    }
  else
    {
      QFile xsl("templates/pdf/main.xsl");
      xsl.copy(output + "main.xsl");
    }
}

bool Loader::load_xml(QString xml_file, bool html, QString output)
{
  QFile file(xml_file);

  if (file.exists())
    {
      file.open(QIODevice::ReadOnly);
      set_output(output);

      QFile out_file(output + "output.xml");
      out_file.open(QIODevice::ReadWrite);

      QTextStream stream_in(&file);
      QTextStream stream_out(&out_file);

      QString line = stream_in.readLine();

      while(!line.contains("<?xml"))
	line = stream_in.readLine();

      stream_out << line;

      if (html)
	stream_out << "\n<?xml-stylesheet type=\"text/xsl\" href=\"xsl.xsl\" ?>";

      // /!\ attributes of ICDAR PcGts removed.
      while(!line.contains("<pcGts"))
	line = stream_in.readLine();

      line = stream_in.readLine();
      stream_out << "\n<pcGts>";

      while (!line.contains("</pcGts>"))
	{
	  stream_out << "\n" << line;
	  line = stream_in.readLine();
	}

      stream_out << "\n" << line;

      file.close();
      out_file.close();

      return true;
    }
  else
    {
      qDebug() << xml_file + " doesn't exist";
      return false;
    }
}
