#include <sstream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/accu/math/count.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/math/sum.hh>
#include <mln/accu/image/all.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/all.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>

#include <mln/math/abs.hh>
#include <mln/pw/all.hh>
#include <mln/trait/concrete.hh>
#include <mln/util/array.hh>

#include <cmath>

using namespace mln;
using value::int_u8;



// FIXME: Protoype is wrong. Should be Image<I>.
template <typename I>
inline
void
save_tmax_map(util::array<image2d<I> >& arr_ima, unsigned dim3, const char* desc)
{
  image2d<float> ima_c;
  initialize(ima_c, arr_ima[0]);
  data::fill(ima_c, 0.0);
  image2d<unsigned> ima_t;
  initialize(ima_t, ima_c);
  data::fill(ima_t, 0);

  mln_piter(image2d<float>) p(ima_c.domain());
  for_all(p)
    for (unsigned k = 0; k < dim3; ++k)
      if (ima_c(p) < arr_ima[k](p))
      {
	ima_c(p) = arr_ima[k](p);
	ima_t(p) = k;
      }

  std::ostringstream smax;
  smax << desc << "_max.pgm";
  std::ostringstream stime;
  stime << desc << "_time.pgm";

  image2d<int_u8> result_c = data::stretch(int_u8(), ima_c);
  image2d<int_u8> result_t = data::stretch(int_u8(), ima_t);
  io::pgm::save(result_c, smax.str());
  io::pgm::save(result_t, stime.str());
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    std::cout << "  input: input 3D image in dump format encoded in float" << std::endl;
    return 1;
  }

  image3d<float> input;
  io::dump::load(input, argv[1]);
  util::array<image2d<float> > arr_ima;
  for (unsigned int i = 0; i < input.nslices(); ++i)
    arr_ima.append(duplicate(cast_image<float>(slice(input, i))));

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

  save_tmax_map(arr_ima, dim3, "tmax");
  save_tmax_map(arr_smooth, dim3, "tmax_smooth");

  return 0;
}
