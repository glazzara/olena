/*
** Creating a piter onb a slice causes bugs. The iterator takes
** erroneous values.
*/

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/util/array.hh>



int main(int, char*argv[])
{
  using namespace mln;
  using value::int_u12;

  image3d<int_u12> ima(150, 256, 256);
  image2d<util::array<int_u12> > ima_arr(ima.nrows(), ima.ncols());
  mln_piter_(image2d<int_u12>) p(slice(ima, 0).domain());
  for (int i = 0; i < ima.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(ima, i));
    for_all(p)
    {
      ima_arr(p).append(tmp_slice(p));
    }
  }
}
