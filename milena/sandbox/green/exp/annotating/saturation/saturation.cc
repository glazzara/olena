// SATURATION TEST CF MILLET 2008

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/stat/histo1d.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/math/max.hh>
#include <mln/math/min.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_saturation_map.hh>

#include <mln/io/ppm/load.hh>
//#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rgb8.hh>

template <typename I>
unsigned count_histo(const mln::Image<I>& img_)
{
  const   I& img = exact(img_);

  mln_precondition(img.is_valid());

  unsigned     result = 0;

  mln_piter(I) p(img.domain());

  for_all(p)
    result += img(p);

  return result;
}


float saturation_test(const std::string input,
		      const std::string output,
//		      const std::string tmp,
		      const unsigned threshold)

{
  typedef mln::fun::v2v::rgb_to_saturation_map<8>   t_rgb_to_saturation_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  unsigned                         cnt1;
  unsigned                         cnt2;
  float                            prop;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map    = mln::data::transform(input_rgb8, t_rgb_to_saturation_map());
  histo  = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  cnt1   = count_histo(histo | mln::box1d(mln::point1d(0),
					  mln::point1d(threshold)));
  cnt2   = mln::geom::nsites(input_rgb8);
  prop   = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo, output.c_str());
//  mln::io::pgm::save(map, tmp.c_str());

  return prop;
}


int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path[] = {t_path(ICDAR_20P_PPM_IMG_PATH)};

  for (int i = 0; i < 1; ++i)
  {
    std::cout << "entering "   << full_path[i] << std::endl;

    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      float             prop = 0.0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	// concatenation de chaine
	t_path directory(ANNOTATING_SATURATION_RET_PATH);
	t_path leaf   = dir_iter->path().leaf();
	t_path output = change_extension(directory / leaf, ".sh");
	t_path tmp    = change_extension(directory / leaf, ".pgm");

	prop = saturation_test(dir_iter->path().string(),
			       output.string(),
//			       tmp.string(),
			       25);

	std::cout << output << " : " << prop << std::endl;
	std::cerr << output << " : " << prop << std::endl;
      }
    }
  }

  return 0;
}
