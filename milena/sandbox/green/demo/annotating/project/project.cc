// Test de l'opérateur de Millet PROJECT
// subsampling the image sur 100x100
// Sobel, puis seuillage à 100
// division horizontale de l'image, puis projection verticale des pixels
// division verticale puis projection horizontale
// assemblage des 4 vecteurs de taille 100

#include <iostream>
#include <sstream>

#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>
#include <mln/accu/stat/mean.hh>

#include <mln/binarization/threshold.hh>

#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/initialize.hh>
#include <mln/core/image/dmorph/unproject_image.hh>



#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/paste.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/projection.hh>

#include <mln/img_path.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

#include <mln/linear/sobel_2d.hh>

#include <mln/opt/at.hh>

const char *get_name(const char *base, const unsigned i, const unsigned j)
{
  std::ostringstream name;

  name << base;
  name << i;
  name << "_";
  name << j;
  name << ".ppm";

  return name.str().c_str();
}


mln::image1d<mln::value::int_u8>
project_row(const mln::image2d<mln::value::int_u8>& img)
{
  typedef mln::accu::math::sum<mln::value::int_u8,mln::value::int_u8> t_sum;
  typedef mln::fun::v2v::projection<mln::point2d,0> t_projection;

  mln::image1d<t_sum> img_accu(img.ncols());

  mln::accu::image::init(img_accu);

  mln::accu::image::take(unproject(img_accu,
			      img.domain(),
			      t_projection()).rw(),
		    img);

  return mln::accu::image::to_result(img_accu);
}

mln::image1d<mln::value::int_u8>
project_col(const mln::image2d<mln::value::int_u8>& img)
{
  typedef mln::accu::math::sum<mln::value::int_u8,mln::value::int_u8> t_sum;
  typedef mln::fun::v2v::projection<mln::point2d,1> t_projection;

  mln::image1d<t_sum> img_accu(img.nrows());

  mln::accu::image::init(img_accu);

  mln::accu::image::take(unproject(img_accu,
			      img.domain(),
			      t_projection()).rw(),
		    img);

  return mln::accu::image::to_result(img_accu);
}


int main()
{
  typedef mln::value::int_u8                      t_int_u8;
  typedef mln::algebra::vec<3,float>              t_vec3f;
  typedef mln::algebra::vec<3,unsigned>           t_vec3u;
  typedef mln::image2d<float>                     t_image2d_float;
  typedef mln::image2d<bool>                      t_image2d_bool;
  typedef mln::image2d<t_int_u8>                  t_image2d_int_u8;
  typedef mln::accu::meta::stat::mean             t_mean_fun;
  typedef mln::accu::math::sum<t_int_u8,t_int_u8> t_sum;
  typedef mln::image1d<t_int_u8>                  t_image1d_int_u8;

  t_image2d_int_u8                                input_int_u8;
  t_image2d_float                                 sobel;
  t_image2d_bool                                  thresh_bool;
  t_image2d_int_u8                                thresh_int_u8;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::pgm::load(input_int_u8,SCRIBO_PGM_IMG_PATH"/mp00082c_50p_8bits.pgm");


  // IMAGE SPLITTING PHASE
  t_image2d_int_u8 subimg_int_u8(mln::box2d(mln::point2d(0,0),
					    mln::point2d(100,100)));

  mln::box2d     domain = input_int_u8.domain();
  mln::point2d   pmin   = domain.pmin();
  mln::point2d   pmax   = domain.pmax();

  unsigned       sz_row = (pmax.row() - pmin.row())/ 100;
  unsigned       sz_col = (pmax.col() - pmin.col())/ 100;

  std::cout << domain << std::endl;

  // Subsampling in 100x100
  // FIXME Test that is it a subsampling, not a upsampling!
  for (unsigned i = 0; i < 100; ++i)
    for (unsigned j = 0; j < 100; ++j)
    {
      mln::point2d min(pmin.row()+sz_row*i,pmin.col()+sz_col*j);
      mln::point2d max(pmin.row()+sz_row*(i+1),pmin.col()+sz_col*(j+1));
      mln::box2d   dom(min,max);

      std::cout << dom << std::endl;

      // Save it
      t_image2d_int_u8 input_part_int_u8(dom);

      mln::data::paste(input_int_u8 | dom, input_part_int_u8);
      //mln::io::pgm::save(input_part_int_u8, get_name("output",i,j));
      float    mean     = mln::data::compute(t_mean_fun(), input_part_int_u8);
      t_int_u8 val      = static_cast<t_int_u8>(mean);
      mln::opt::at(subimg_int_u8, i,j) = val;
    }

  mln::io::pgm::save(subimg_int_u8, "subimg.pgm");

  sobel         = mln::linear::sobel_2d_l1_norm(subimg_int_u8);
  thresh_bool   = mln::binarization::threshold(sobel, 100);
  thresh_int_u8 = mln::data::convert(t_int_u8(), thresh_bool);

  mln::io::pbm::save(thresh_bool, "subimg.pbm");

  // Define img_up, img_down, img_left, img_right
  t_image2d_int_u8 img_up(mln::box2d(mln::point2d(0,0),
				     mln::point2d(49,99)));

  mln::data::paste(thresh_int_u8 | img_up.domain(), img_up);


  t_image2d_int_u8 img_down(mln::box2d(mln::point2d(50,0),
				       mln::point2d(99,99)));

  mln::data::paste(thresh_int_u8 | img_down.domain(), img_down);

  t_image2d_int_u8 img_left(mln::box2d(mln::point2d(0,0),
				       mln::point2d(99,49)));

  mln::data::paste(thresh_int_u8 | img_left.domain(), img_left);


  t_image2d_int_u8 img_right(mln::box2d(mln::point2d(0,50),
					mln::point2d(99,99)));

  mln::data::paste(thresh_int_u8 | img_right.domain(), img_right);


  // Define project_up, project_down, project_left, project_right
  t_image1d_int_u8 project_up    = project_row(img_up);
  t_image1d_int_u8 project_down  = project_row(img_down);
  t_image1d_int_u8 project_left  = project_col(img_left);
  t_image1d_int_u8 project_right = project_col(img_right);

  mln::io::pgm::save(img_up, "up.pgm");
  mln::io::pgm::save(img_down, "down.pgm");
  mln::io::pgm::save(img_left, "left.pgm");
  mln::io::pgm::save(img_right, "right.pgm");

  // PRINTING PHASE

  std::cout << std::endl << "UP" << std::endl;
  mln::debug::println(project_up);

  std::cout << std::endl << "DOWN" << std::endl;
  mln::debug::println(project_down);

  std::cout << std::endl << "LEFT" << std::endl;
  mln::debug::println(project_left);

  std::cout << std::endl << "RIGHT" << std::endl;
  mln::debug::println(project_right);
}
