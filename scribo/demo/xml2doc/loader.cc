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

# include "loader.hh"

Loader::Loader()
{
}

Loader::~Loader()
{
}

QDomDocument* Loader::xml_to_dom(const QString& xml_file)
{
  QFile file(xml_file);

  if (file.open(QIODevice::ReadOnly))
    {
      QDomDocument* document = new QDomDocument;
      if (document->setContent(&file))
	{
	  file.close();
	  return document;
	}
    }

  file.close();
  return 0;
}

bool Loader::set_output(QString& output)
{
  QDir dir(output);

  if (!output.endsWith("/"))
    output.append("/");

  if (!dir.exists())
    return dir.mkpath(".");
  else
    {
      QStringList list = dir.entryList(QDir::Writable | QDir::AllDirs);
      return list.size() != 0;
    }
}


void Loader::add_html_templates(bool base64, const QString& output)
{
  QFile gen("templates/html/html_generator.sh");
  gen.copy(output + "html_generator.sh");

  QFile css("templates/html/css.css");
  css.copy(output + "css.css");

  if (base64)
    {
      QFile xsl("templates/html/xsl_base64.xsl");
      xsl.copy(output + "xsl.xsl");
    }
  else
    {
      QFile xsl("templates/html/xsl.xsl");
      xsl.copy(output + "xsl.xsl");
    }
}

void Loader::add_pdf_templates(bool crop, bool base64, const QString& output)
{
  if (base64)
    {
      QFile regions("templates/pdf/regions_base64.xsl");
      regions.copy(output + "regions.xsl");
    }
  else
    {
      QFile regions("templates/pdf/regions_png.xsl");
      regions.copy(output + "regions.xsl");
    }

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
      if (base64)
	{
	  QFile xsl("templates/pdf/main.xsl");
	  xsl.copy(output + "main.xsl");
	}
      else
	{
	  QFile xsl("templates/pdf/main64.xsl");
	  xsl.copy(output + "main.xsl");
	}
    }
}

void Loader::add_svg_templates(const QString& output)
{
  QFile regions("templates/pdf/regions_svg.xsl");
  regions.copy(output + "regions.xsl");

  QFile gen("templates/pdf/svg_generator.sh");
  gen.copy(output + "svg_generator.sh");

  QFile line("templates/pdf/line.xsl");
  line.copy(output + "line.xsl");

  QFile xsl("templates/pdf/main_crop.xsl");
  xsl.copy(output + "main.xsl");
}

void Loader::add_open_templates(const QString& output)
{
  QFile css("templates/opendoc/css.css");
  css.copy(output + "css.css");

  QFile gen("templates/opendoc/opendoc_generator.sh");
  gen.copy(output + "opendoc_generator.sh");

  QFile xsl("templates/opendoc/xsl.xsl");
  xsl.copy(output + "xsl.xsl");
}

bool Loader::xml_output(const QString& xml_file, bool html, const QString& output)
{
  QFile file(xml_file);

  file.open(QIODevice::ReadOnly);

  QFile out_file(output + "output.xml");
  out_file.open(QIODevice::ReadWrite);

  QTextStream stream_in(&file);
  QTextStream stream_out(&out_file);
  stream_in.setCodec("UTF-8");
  stream_out.setCodec("UTF-8");

  QString line = stream_in.readLine();

  while(!line.contains("<?xml"))
    line = stream_in.readLine();

  stream_out << line;

  if (html)
    stream_out << "\n<?xml-stylesheet type=\"text/xsl\" href=\"xsl.xsl\" ?>";

  // /!\ attributes of ICDAR PcGts removed.
  while(!line.contains("<PcGts"))
    line = stream_in.readLine();

  line = stream_in.readLine();
  stream_out << "\n<PcGts>";

  while (!line.contains("</PcGts>"))
    {
      stream_out << "\n" << line;
      line = stream_in.readLine();
    }

  stream_out << "\n" << line;

   file.close();
   out_file.close();

  return true;
}
