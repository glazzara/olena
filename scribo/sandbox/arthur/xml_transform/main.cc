#include <iostream>
#include "xml_transform.hh"

int main(int argc, char **argv)
{

  std::string man = "xml_transform <option> <xml_file> <ppm_file> <output_dir>\nOPTIONS:\n\t--html: Produce a HTML output\n\t--pdf: Produce a PDF output, regions will be croped as follows in the XML file.\n\t--pdf-no-crop: Produce a PDF output without croping regions, the entire picture will be displayed.";

  if (argc > 4)
    {
      std::string html = "--html";
      std::string pdf = "--pdf";
      std::string pdf_no_crop = "--pdf-no-crop";

      XmlTransform xmlt(argv[4], argv[2]);

      if (html.compare(argv[1]) == 0)
	xmlt.createHTML(argv[3]);
      else if (pdf.compare(argv[1]) == 0)
	xmlt.createPDF(argv[3], true);
      else if (pdf_no_crop.compare(argv[1]) == 0)
	xmlt.createPDF(argv[3], false);
      else
	std::cout << man << std::endl;
    }
  else
    std::cout << man << std::endl;

  return 0;
}
