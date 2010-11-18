#include <iostream>
#include <string>
#include <string>
#include <cstdlib>
#include "xml_transform.hh"

int main(int argc, char **argv)
{

  std::string man;
  man = "xml_transform\n"
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
    "\t\t\tCreates Open Document Writer (odt) file.\n\n"

    "Base 64 operations:\n"
    "\t\033[01m--to-base64\033[00m \033[04minput.xml\033[00m \033[04mimage\033[00m \033[04moutput.xml\033[00m"
    "\t\t\tCreates a container XML file. It will contain regions which are cropped and converted in base 64.\n"

    "\t\033[01m--to-base64-no-crop\033[00m \033[04minput.xml\033[00m \033[04image\033[00m \033[04moutput.xml\033[00m"
    "\t\tSame as --to-base64 but only the full picture will be converted, regions are not cropped\n"

    "\t\033[01m--from-base64\033[00m \033[04minput.xml\033[00m \033[04moutput_dir\033[00m"
    "\t\t\tDecodes a XML file which has been encoded in base 64, regions will be converted into png files and saved in output_dir.\n\n"

    "Requirements:\n"
    "PDF : fop >= 0.95\n"
    "HTML : xsltproc\n"
    "SVG : trunk version of fop\n"
    "OpenDocument : ooconvert (included)";
  if (argc > 4)
    {
      std::string html = "--html";
      std::string html_full = "--html-full";
      std::string open = "--oo-doc";
      std::string pdf = "--pdf";
      std::string svg = "--svg";
      std::string to_base64 = "--to-base64";
      std::string to_base64nocrop = "--to-base64-no-crop";

      if (html_full.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], argv[4]);
	  xmlt.createHTML(false);

	  QString cmd = "sh templates/xml_transform.sh html " + xmlt.out() + "output.xml " +  xmlt.out() + "output.html";
	  return system (cmd.toAscii().constData());
	}
      else if (html.compare(argv[1]) == 0)
	{
	  QTemporaryFile tmp;
	  tmp.open();
	  tmp.fileName();

	  XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
	  xmlt.toBase64(false);

	  QString cmd = "sh templates/xml_transform.sh html-base64 " + tmp.fileName() + " " + argv[4];
	  return system (cmd.toAscii().constData());
	}
      else if (pdf.compare(argv[1]) == 0)
	{
	  QTemporaryFile tmp;
	  tmp.open();
	  tmp.fileName();

	  XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
	  xmlt.toBase64(false);

	  QString cmd = "sh templates/xml_transform.sh pdf " + tmp.fileName() + " " + argv[4];
	  return system (cmd.toAscii().constData());
	}
      else if (svg.compare(argv[1]) == 0)
	{
	  QTemporaryFile tmp;
	  tmp.open();
	  tmp.fileName();

	  XmlTransform xmlt(argv[2], argv[3], QString::Null(), tmp.fileName());
	  xmlt.toBase64(false);

	  QString cmd = "sh templates/xml_transform.sh svg " + tmp.fileName() + " " + argv[4];
	  return system (cmd.toAscii().constData());
	}
      else if (open.compare(argv[1]) == 0)
	{
	  QString dir = QDir::tempPath() + "/xml_transform." + argv[4];
	  XmlTransform xmlt(argv[2], argv[3], dir);
	  xmlt.createOpen();

	  QString cmd = "sh templates/xml_transform.sh oo-doc " + dir + " " + argv[4];
	  return system (cmd.toAscii().constData());
	}
      else if (to_base64.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], QString::Null(), argv[4]);
	  xmlt.toBase64(false);
	}
      else if (to_base64nocrop.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], QString::Null(), argv[4]);
	  xmlt.toBase64(true);
	}
      else
	std::cout << man << std::endl;
    }
  else if (argc > 3)
    {
      std::string pdf_base64 = "--pdf-base64";
      std::string svg_base64 = "--svg-base64";
      std::string html_base64 = "--html-base64";
      std::string from_base64 = "--from-base64";

      if (svg_base64.compare(argv[1]) == 0)
	{
	  QString cmd = "sh templates/xml_transform.sh svg " + QString(argv[2]) + " " + QString(argv[3]);
	  return system (cmd.toAscii().constData());
	}
      else if (pdf_base64.compare(argv[1]) == 0)
	{
	  QString cmd = "sh templates/xml_transform.sh pdf " + QString(argv[2]) + " " + QString(argv[3]);
	  return system (cmd.toAscii().constData());
	}
      else if (html_base64.compare(argv[1]) == 0)
	{
	  QString cmd = "sh templates/xml_transform.sh html-base64 " + QString(argv[2]) + " " + QString(argv[3]);
	  return system (cmd.toAscii().constData());
	}
      else if (from_base64.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
	  xmlt.fromBase64();
	}
      else
	std::cout << man << std::endl;
    }
  else
    std::cout << man << std::endl;

  return 0;
}
