// TOOLS ==> Color histogram

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/display/display_histo.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/dump/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/opt/at.hh>

#include <mln/pw/value.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>


template <unsigned n>
void mk_histo(const std::string& input,
	      const std::string& output,
	      const std::string& histo,
	      const std::string& mask)
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image2d<bool>                          t_image2d_bool;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  // START OF IMAGE PROCESSING CHAIN
  t_image2d_rgb8                                      i0_input; // input rgb8
  t_image2d_rgbn                                      i1_input; // input rgbn
  t_image2d_bool                                      m0_input; // mask input
  t_histo3d                                           h1_input; // histo input
  t_image2d_int_u8                                    p1_histo; // histo proj

  mln::io::ppm::load(i0_input, input.c_str());
  i1_input  = mln::data::transform(i0_input, t_rgb8_to_rgbn());

  if (0 < mask.size())
  {
    mln::io::pbm::load(m0_input, mask.c_str());
    h1_input  = mln::data::compute(t_histo3d_fun(),
				   (i1_input | mln::pw::value(m0_input)).rw());
  }
  else
  {
    h1_input  = mln::data::compute(t_histo3d_fun(), i1_input);
  }
  // END OF IMAGE PROCESSING CHAIN

  // BEGIN DUMPING
  p1_histo = mln::display::display_histo3d_unsigned(h1_input);
  mln::io::dump::save(h1_input, histo.c_str());
  mln::io::pgm::save(p1_histo, output.c_str());
  // END DUMPING
}


void usage()
{
  std::cout                                                       << std::endl;
  std::cout << "histo input.ppm q out.ppm histo.dump [msk.pbm]"   << std::endl;
  std::cout << "where"                                            << std::endl;
  std::cout << "input.ppm is the 8 bits color ppm image"          << std::endl;
  std::cout << "q is the degree of quanification {2,3,4,5,6,7,8}" << std::endl;
  std::cout << "out.pgm is the r/g projection of the histogram"   << std::endl;
  std::cout << "out.dump is the quantified color histogram"       << std::endl;
  std::cout << "msk.pbm is the mask which select the pixels"      << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* args[])
{
  if (5 == argc || 6 == argc)
  {
    const std::string input(args[1]);
    const char        q = args[2][0];
    const std::string output(args[3]);
    const std::string histo(args[4]);
    const std::string mask(6 == argc? args[5] : "");

    switch(q)
    {
      case '2': mk_histo<2>(input, output, histo, mask); break;
      case '3': mk_histo<3>(input, output, histo, mask); break;
      case '4': mk_histo<4>(input, output, histo, mask); break;
      case '5': mk_histo<5>(input, output, histo, mask); break;
      case '6': mk_histo<6>(input, output, histo, mask); break;
      case '7': mk_histo<7>(input, output, histo, mask); break;
      case '8': mk_histo<8>(input, output, histo, mask); break;
      default:  usage(); break;
    }
  }
  else
    usage();

  return 0;
}
