#include <iostream>
#include "xml_to_html.hh"

int main(int argc, char **argv)
{

  std::string man;
  man = "xml_to_html: ...usage\n -m, --merge: Merge an XML with an other XML\n \targs: <xml_file_1> <xml_file_2> [xml_output]\n\n -c, --create-html: Crop images (if exist) from the ppm file using the XML file and produce an html output.\n \targs: <xml_file> [ppm_file] [output dir]\n";

  if (argc > 2)
    {
      new QApplication::QApplication(argc, argv, false);
      ImageCrop crop;

      std::string merge = "--merge";
      std::string m = "-m";
      std::string scrop = "--create-html";
      std::string c = "-c";

      if (argc == 3 && (scrop.compare(argv[1]) == 0 || c.compare(argv[1]) == 0))
	{
	  bool b = false;
	  b = crop.load_xml(argv[2], QString::Null());

	  if (b)
	    crop.find_image_regions();
	}
      else if (argc > 3)
	{
	  if (merge.compare(argv[1]) == 0 || m.compare(argv[1]) == 0)
	    {
	      if (argc > 4)
		crop.merge(argv[2], argv[3], argv[4]);
	      else
		crop.merge(argv[2], argv[3]);
	    }
	  else if (scrop.compare(argv[1]) == 0 || c.compare(argv[1]) == 0)
	    {
	      bool b = false;
	      if (argc > 4)
		b = crop.load_xml(argv[2], argv[3], argv[4]);
	      else
		b = crop.load_xml(argv[2], argv[3]);

	      if (b)
		crop.find_image_regions();
	    }
	  else
	    std::cout << man;
	}
      else
	std::cout << man;
    }
  else
    std::cout << man;

  return 0;
}
