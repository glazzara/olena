#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/fill.hh>
#include <mln/level/stretch.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/mesh_image.hh>
# include <mln/labeling/base.hh>
# include <mln/debug/println.hh>
# include <mln/core/window2d.hh>
# include <mln/convert/to_window.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>



namespace mln
{
  namespace convert
  {
//     template <typename N>
//     window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh_)
//     {
//       const N& nbh = exact(nbh_);
//       typedef mln_dpoint(N) D;
//       typedef mln_point(D) P;
//       window<D> win;
//       mln_niter(N) n(nbh, P::zero);
//       for_all(n)
// 	if (n > P::zero)
// 	  win.insert(n - P::zero);
//       return win;
//     }

    template <typename N>
    window<mln_dpoint(N)> to_lower_window(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_niter(N) n(nbh, P::zero);
      for_all(n)
	if (n < P::zero)
	  win.insert(n - P::zero);
      return win;
    }

  } // end of namespace convert

  template <typename I, typename N, typename J>
  void
  chamfer (Image<I>& ima_, const Neighborhood<N>& nbh_,
	   mln_point(I)& point, unsigned max, Image<J>& out_)
  {
    I& ima = exact (ima_);
    J& out = exact (out_);

    typedef window<mln_dpoint(I)> W;

    /// Init.
    {
      level::fill (out, max);
      out(point) = 0;
    } 

    /// Fwd pass.
    {
      mln_fwd_piter (I) p (ima.domain ());
      W win = convert::to_upper_window(nbh_);
      mln_qiter(W) n(win, p);

      for_all (p)
	{
	  for_all (n)
	    {
	      if (out(n) > out(p) + 1)
		out(n) = out(p) + 1;
	    }
	}
    }

    /// Bkd pass.
    {
      mln_bkd_piter (I) p (ima.domain ());
      W win = convert::to_lower_window(nbh_);
      mln_qiter(W) n( win, p);

      for_all (p)
	{
	  for_all (n)
	    {
	      if (out(n) > out(p) + 1)
		out(n) = out(p) + 1;
	    }
	}
    }
  }
} // end of mln

int main ()
{
  using namespace mln;
  using value::int_u8;

  {
  image2d_b<int_u8> ima (9, 9);
  image2d_b<unsigned> out (ima.domain ());
  point2d p = make::point2d(4,4);

  chamfer (ima, c4(), p, 255, out);
  debug::println (out);
  }



  {
  image2d_b<int_u8> ima (1000, 1000);
  image2d_b<unsigned> out (ima.domain ());
  point2d p = make::point2d(4,4);

  chamfer (ima, c4(), p, 255, out);
  }
  //  debug::println (out);
}
