#include <iostream>
#include <QDomDocument>
#include "xml_transform.hh"

int main(int argc, char **argv)
{

  std::string man;
  man = "xml_transform\n"
    "OPTIONS:\n\n"
    "HTML output:\n"
    "\t--html <xml> <ppm> <out_dir> : HTML output with non-text regions cropped into many png files.\n"
    "\t--html-base64 <xml> <out_dir> : HTML output from a container XML file.\n\n"
    "PDF output:\n"
    "\t--pdf <xml> <ppm> <out_dir> : PDF output with non-text regions cropped into many png files.\n"
    "\t--pdf-no-crop <xml> <ppm> <out_dir>: PDF output with the entire picture displayed over the text.\n"
    "\t--pdf-base64 <xml> <out_dir> : PDF output from a container XML file.\n"
    "\t--pdf-base64-no-crop <xml> <ppm> <out_dir> : PDF with the entire picture displayed over the text, the picture is loaded from a container XML file.\n\n"
    "SVG output:\n"
    "\t--svg <xml> <out_dir> : SVG output (works only with base 64 xml encoded.\n\n"
    "Base 64 operations:\n"
    "\t--to-base64 <xml> <ppm> <out_xml> : Produces a container XML file by converting cropped pictures into base 64 format.\n\n"

    "OpenDocument output:\n"
    "\t--open <xml> <ppm> <out_dir> : OpenDocument output (odt, odf).\n\n"
    "Base 64 operations:\n"
    "\t--to-base64 <xml> <ppm> <out_xml> : Produces a container XML file by converting cropped pictures into base 64 format.\n"
    "\t--to-base64-no-crop <xml> <ppm> <out_xml> : Same as to-base64 but only the picture is converted in base 64.\n"
    "\t--from-base64 <xml> <out_dir> : Decodes a XML container file to produce original image files.\n\n"
    "For PDF/HTML/SVG/OD output, Bourne shell scripts are provided to produce the outputs, just run sh *_generator.sh \n"
    "Requirements:\n"
    "PDF : fop >= 0.95\n"
    "HTML : xsltproc\n"
    "SVG : trunk version of fop\n"
    "OpenDocument : ooconvert";

  if (argc > 4)
    {
      std::string html = "--html";
      std::string open = "--open";
      std::string pdf = "--pdf";
      std::string pdf_no_crop = "--pdf-no-crop";
      std::string to_base64 = "--to-base64";
      std::string to_base64nocrop = "--to-base64-no-crop";

      if (html.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], argv[4]);
	  xmlt.createHTML(false);
	}
      else if (pdf.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], argv[4]);
	  xmlt.createPDF(true, false);
	}
      else if (open.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], argv[4]);
	  xmlt.createOpen();
	}
      else if (pdf_no_crop.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], argv[3], argv[4]);
	  xmlt.createPDF(false, false);
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
      std::string pdfbase64 = "--pdf-base64";
      std::string svg = "--svg";
      std::string pdfbase64nocrop = "--pdf-base64-no-crop";
      std::string htmlbase64 = "--html-base64";
      std::string from_base64 = "--from-base64";

      if (pdfbase64.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
	  xmlt.createPDF(true, true);
	}
      else if (svg.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
	  xmlt.createSVG();
	}
      else if (pdfbase64nocrop.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
	  xmlt.createPDF(false, true);
	}
      else if (htmlbase64.compare(argv[1]) == 0)
	{
	  XmlTransform xmlt(argv[2], QString::Null(), argv[3]);
	  xmlt.createHTML(true);
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
