# include <mln/core/w_window2d_int.hh>
# include <mln/core/w_window2d_float.hh>
# include <math.h>

namespace mln
{
  namespace win_chamfer
  {

    template<int d10, int d11>
    const w_window2d_int
    mk_chamfer_3x3_int()
    {
      int ws[] = { d11, d10, d11,
	           d10,   0,   0,
		     0,   0,   0 };

      return (make::w_window2d(ws));
    }

    template<int d10, int d11, int d21>
    const w_window2d_int
    mk_chamfer_5x5_int()
    {
      int ws[] = {   0, d21,   0, d21,   0,
         	   d21, d11, d10, d11, d21,
		     0, d10,   0,   0,   0,
	             0,   0,   0,   0,   0,
		     0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const w_window2d_float
    mk_chamfer_3x3_float(float d10, float d11)
    {
      float ws[] = { d11, d10, d11,
      	             d10,   0,   0,
		       0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const w_window2d_float
    mk_chamfer_5x5_float(float d10, float d11, float d21)
    {
      float ws[] = {   0, d21,   0, d21,   0,
         	     d21, d11, d10, d11, d21,
		       0, d10,   0,   0,   0,
	               0,   0,   0,   0,   0,
  		       0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const w_window2d_float
    mk_chamfer_exact()
    {
      float r2 = sqrt(2);
      float ws[] = {  r2,   1,  r2,
      	               1,   0,   0,
		       0,   0,   0 };

      return (make::w_window2d(ws));
    }

  } // end of mln::win_chamfer

} // end of mln
