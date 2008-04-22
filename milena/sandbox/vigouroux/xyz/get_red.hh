#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "my_xyz.hh"

#ifndef MLN_XYZ_FUN_HH
# define MLN_XYZ_FUN_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_xyz_ : public Function_v2v< f_xyz_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_xyz>
	T_rgb operator()(const T_xyz& xyz) const
	{
           int r;
           
           r = int(xyz.y() + 1.13983 * xyz.y());
             
           struct value::rgb<8> rgb(r, 0, 0); 
           
           return rgb;
	}
      };

      typedef f_xyz_<value::rgb8> f_xyz_get_red;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_VALUE_XYZ_HH
