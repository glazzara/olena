#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>
#include <mln/metal/is_a.hh>

#include "color/my_hsi.hh"
#include <mln/value/rgb.hh>

#include "hsi/fun.hh"
#include "cmy/fun.hh"
#include "yuv/fun.hh"
#include "yiq/fun.hh"

namespace mln
{


  namespace fun
  {

    namespace v2v
    {

      template <typename T_red>
      struct f_get_red_ : public Function_v2v< f_get_red_<T_red> >
      {
            typedef T_red result;

	template <typename T_col>
	T_red operator()(const T_col& col) const
	{
          T_red r;

	  if (mlc_is_a(T_col, mln::value::HSI)::value == 1)
          {
             r = mln::fun::v2v::f_hsi_3x8(col);
             std::cout << "hsi" << std::endl;
          }
// 	  else if (mlc_is_a(T_col, mln::value::CMY)::value == 1)
//           {
//              r = mln::fun::v2v::f_cmy_3x8(col);
//              std::cout << "cmy" << std::endl;
//           }
// 	  if (mlc_is_a(T_col, mln::value::YUV)::value == 1)
//           {
//              r = mln::fun::v2v::f_yuv_3x8(col);
//              std::cout << "yuv" << std::endl;
//           }
// 	  else if (mlc_is_a(T_col, mln::value::YIQ)::value == 1)
//           {
//              r = mln::fun::v2v::f_yiq_3x8(col);
//              std::cout << "yiq" << std::endl;
//           }
          else
          {
//              r = mln::fun::v2v::f_cmy_3x8(col);
             std::cout << "else" << std::endl;
          }
	  return r;
	}
      };

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln
