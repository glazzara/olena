#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/magick/save.hh>

#include <mln/accu/count.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/sum.hh>
#include <mln/accu/image/all.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/all.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

#include <mln/level/compute.hh>
#include <mln/level/stretch.hh>

#include <mln/math/abs.hh>
#include <mln/pw/all.hh>
#include <mln/trait/concrete.hh>
#include <mln/util/array.hh>

#include <cmath>

using namespace mln;
using value::int_u8;


inline
image2d<float> mean_slices(util::array<image2d<float> >& arr_ima, int first, int last)
{
  image2d<accu::mean<float> > result;

  mln_precondition(first >=0 && first < arr_ima.nelements());
  mln_precondition(last >=0 && last < arr_ima.nelements());

  initialize(result, arr_ima[first]);

  accu::image::init(result);
  for (int i = first; i < last; ++i)
    accu::image::take(result, arr_ima[i]);

  return accu::image::to_result(result);
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    return 1;
  }

  image3d<value::int_u12> input;
  io::dicom::load(input, argv[1]);
  util::array<image2d<float> > arr_ima;
  for (unsigned int i = 0; i < input.nslices(); ++i)
    arr_ima.append(duplicate(cast_image<float>(slice(input, i))));

  // Calcul signal initialmoyen : 8 images de 2 à 9 moyennées=ligne de base
  image2d<float> imageini = mean_slices(arr_ima, 1, 8);

  unsigned dim3 = arr_ima.nelements();


  /////////////
  //	     //
  // Lissage //
  //	     //
  /////////////

  util::array<image2d<double> > arr_smooth;
  arr_smooth.append(arr_ima[0] * 1.0);
  for (unsigned k = 1; k < dim3 - 1; ++k)
    arr_smooth.append(arr_ima[k] * 0.5 + arr_ima[k - 1] * 0.25 + arr_ima[k + 1] * 0.25);
  arr_smooth.append(arr_ima[dim3 - 1] * 1.0);


  ///////////////////////////////////
  //				   //
  // Calcul image max et temps max //
  //				   //
  ///////////////////////////////////

  image2d<float> ima_c;
  initialize(ima_c, arr_smooth[0]);
  data::fill(ima_c, 0.0);
  image2d<unsigned> ima_t;
  initialize(ima_t, ima_c);
  data::fill(ima_c, 0);
  mln_piter_(image2d<float>) p(ima_c.domain());
  for_all(p)
    for (unsigned k = 0; k < dim3; ++k)
      if (ima_c(p) < arr_ima[k](p))
      {
	ima_c(p) = arr_ima[k](p);
	ima_t(p) = k;
      }

  image2d<int_u8> result_c = level::stretch(int_u8(), ima_c);
  image2d<int_u8> result_t = level::stretch(int_u8(), ima_t);
  io::magick::save(result_c, "result_max.png");
  io::magick::save(result_t, "result_time.png");

  return 0;
}
