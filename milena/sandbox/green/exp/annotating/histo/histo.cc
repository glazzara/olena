// Build normalized histogram as density

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/stat/histo1d.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/div.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/convert.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/literal/zero.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/grays.hh>

#include <mln/math/max.hh>
#include <mln/math/min.hh>
#include <mln/math/sqr.hh>
#include <mln/math/sqrt.hh>

#include <mln/opt/at.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_hue_map.hh>
#include <mln/fun/v2v/rgb_to_saturation_map.hh>
#include <mln/fun/v2v/rgb_to_value_map.hh>
#include <mln/fun/v2v/component.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rgb8.hh>

//============================================================================//
//                         HISTOGRAM
//============================================================================//

template <typename I>
mln_value(I) cnt_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) cnt = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    cnt += histo(p);
  }

  return cnt;
}

void histo(const std::string input,
	   const std::string output_map,
	   const std::string output_histo,
	   const char        space)

{
  typedef mln::value::rgb8                              t_rgb8;
  typedef mln::fun::v2v::rgb_to_hue_map<8>		t_rgb_to_hue_map;
  typedef mln::fun::v2v::rgb_to_saturation_map<8>	t_rgb_to_sat_map;
  typedef mln::fun::v2v::rgb_to_value_map<8>		t_rgb_to_val_map;
  typedef mln::fun::v2v::component<t_rgb8,0>		t_component_r;
  typedef mln::fun::v2v::component<t_rgb8,1>		t_component_g;
  typedef mln::fun::v2v::component<t_rgb8,2>		t_component_b;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            sum;

  mln::io::ppm::load(input_rgb8, input.c_str());

  switch(space)
  {
    case 'h': map = mln::data::transform(input_rgb8, t_rgb_to_hue_map()); break;
    case 's': map = mln::data::transform(input_rgb8, t_rgb_to_sat_map()); break;
    case 'v': map = mln::data::transform(input_rgb8, t_rgb_to_val_map()); break;
    case 'r': map = mln::data::transform(input_rgb8, t_component_r()); break;
    case 'g': map = mln::data::transform(input_rgb8, t_component_g()); break;
    case 'b': map = mln::data::transform(input_rgb8, t_component_b()); break;
    default:  break;// crash
  }

  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = cnt_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;

  mln::io::pgm::save(map, output_map.c_str());
  mln::io::plot::save_image_sh(histo_float, output_histo.c_str());
}

//============================================================================//
//                         MAIN
//============================================================================//


// in the directory of the images
// out the density directory
// 2 use-cases afp and icdar

int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

//   t_path full_path[] = {t_path(ICDAR_20P_TEXT_ONLY_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_COLOR_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_PHOTO_IMG_PATH)};

//  t_path full_path[] = {t_path(AFP_INPUT_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_GMP30_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_GMP20_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_GMP10_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_MGK30_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_MGK20_IMG_PATH)};
//   t_path full_path[] = {t_path(AFP_MGK10_IMG_PATH)};

//  t_path full_path[] = {t_path(ICDAR_20P_INPUT_IMG_PATH)};
//   t_path full_path[] = {t_path(ICDAR_20P_GMP30_IMG_PATH)};
   t_path full_path[] = {t_path(ICDAR_20P_GMP20_IMG_PATH)};
//   t_path full_path[] = {t_path(ICDAR_20P_GMP10_IMG_PATH)};
//   t_path full_path[] = {t_path(ICDAR_20P_MGK30_IMG_PATH)};
//   t_path full_path[] = {t_path(ICDAR_20P_MGK20_IMG_PATH)};
//   t_path full_path[] = {t_path(ICDAR_20P_MGK10_IMG_PATH)};

  for (int i = 0; i < 1; ++i)
  {
    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      t_path            directory;
      t_path            leaf;
      t_path            output_map;
      t_path            output_histo;

      std::cerr << "entering "   << full_path[i] << std::endl;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	std::cerr << dir_iter->path() << std::endl;

	leaf         = dir_iter->path().leaf();

//	directory    = ANNOTATING_AFP_R_INPUT_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_GMP30_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_R_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_R_INPUT_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_R_GMP30_RET_PATH;
 	directory    = ANNOTATING_ICDAR_R_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_R_GMP10_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_R_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_R_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_R_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      'r');

//	directory    = ANNOTATING_AFP_G_INPUT_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_GMP30_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_G_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_G_INPUT_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_G_GMP30_RET_PATH;
 	directory    = ANNOTATING_ICDAR_G_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_G_GMP10_RET_PATH;
//	directory    = ANNOTATING_ICDAR_G_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_G_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_G_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      'g');

//	directory    = ANNOTATING_AFP_B_INPUT_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_GMP30_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_B_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_B_INPUT_RET_PATH;
//	directory    = ANNOTATING_ICDAR_B_GMP30_RET_PATH;
 	directory    = ANNOTATING_ICDAR_B_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_B_GMP10_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_B_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_B_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_B_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      'b');

//	directory    = ANNOTATING_AFP_H_INPUT_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_GMP30_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_H_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_H_INPUT_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_H_GMP30_RET_PATH;
	directory    = ANNOTATING_ICDAR_H_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_H_GMP10_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_H_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_H_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_H_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      'h');

//	directory    = ANNOTATING_AFP_S_INPUT_RET_PATH;
//	directory    = ANNOTATING_AFP_S_GMP30_RET_PATH;
//	directory    = ANNOTATING_AFP_S_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_S_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_S_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_S_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_S_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_S_INPUT_RET_PATH;
//	directory    = ANNOTATING_ICDAR_S_GMP30_RET_PATH;
 	directory    = ANNOTATING_ICDAR_S_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_S_GMP10_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_S_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_S_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_S_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      's');

//	directory    = ANNOTATING_AFP_V_INPUT_RET_PATH;
//	directory    = ANNOTATING_AFP_V_GMP30_RET_PATH;
// 	directory    = ANNOTATING_AFP_V_GMP20_RET_PATH;
// 	directory    = ANNOTATING_AFP_V_GMP10_RET_PATH;
// 	directory    = ANNOTATING_AFP_V_MGK30_RET_PATH;
// 	directory    = ANNOTATING_AFP_V_MGK20_RET_PATH;
// 	directory    = ANNOTATING_AFP_V_MGK10_RET_PATH;

//	directory    = ANNOTATING_ICDAR_V_INPUT_RET_PATH;
//	directory    = ANNOTATING_ICDAR_V_GMP30_RET_PATH;
 	directory    = ANNOTATING_ICDAR_V_GMP20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_V_GMP10_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_V_MGK30_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_V_MGK20_RET_PATH;
// 	directory    = ANNOTATING_ICDAR_V_MGK10_RET_PATH;
	output_histo = change_extension(directory / leaf, ".sh");
	output_map   = change_extension(directory / leaf, ".pgm");

	histo(dir_iter->path().string(),
	      output_map.string(),
	      output_histo.string(),
	      'v');
      }
    }
  }

  return 0;
}
