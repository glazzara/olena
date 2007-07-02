#ifndef MLN_CORE_NEIGHB2D_HH
# define MLN_CORE_NEIGHB2D_HH

# include <cmath>
# include <mln/core/neighb.hh>
# include <mln/core/dpoint2d.hh>


namespace mln
{

  typedef neighb_<dpoint2d> neighb2d;

  const neighb2d& c4();
  const neighb2d& c8();
  const neighb2d& c2_row();
  const neighb2d& c2_col();


# ifndef MLN_INCLUDE_ONLY

  const neighb2d& c4()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	it.insert(mk_dpoint2d(1, 0));
	flower = false;
      }
    return it;
  }

  const neighb2d& c8()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	it.insert(mk_dpoint2d(1,-1));
	it.insert(mk_dpoint2d(1, 0));
	it.insert(mk_dpoint2d(1, 1));
	flower = false;
      }
    return it;
  }

  const neighb2d& c2_row()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	flower = false;
      }
    return it;
  }

  const neighb2d& c2_col()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(1, 0));
	flower = false;
      }
    return it;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_NEIGHB2D_HH
