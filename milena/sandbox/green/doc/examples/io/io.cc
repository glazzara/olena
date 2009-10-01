// The aim of this file is to test the different ways to load and save images.

// NEED SPECIFIC LINUX INSTALL
/*
#include <mln/io/dicom/load.hh>
#include <mln/io/fits/load.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/tiff/load.hh>
*/

#include <mln/io/cloud/load.hh>
#include <mln/io/cloud/save.hh>


#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/io/pbms/load.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/io/pgms/load.hh>

#include <mln/io/pfm/load.hh>
#include <mln/io/pfm/save.hh>

#include <mln/io/pnm/load.hh>
#include <mln/io/pnm/save.hh>

#include <mln/io/pnms/load.hh>

#include <mln/io/plot/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/io/ppms/load.hh>



#include <mln/io/txt/save.hh>

#include <mln/debug/println.hh>

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u16.hh>
#include <mln/img_path.hh>

void test_generic_load()
{
  mln::image2d<mln::value::int_u16> input;

  mln::io::pgm::load(input, OLENA_IMG_PATH"/lena.pgm");
  //  mln::io::off::save(OLENA_IMG_PATH"/lena.off");
}


int main()
{
  test_generic_load();

  return 0;
}
