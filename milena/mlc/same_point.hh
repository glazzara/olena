#ifndef MLN_MLC_SAME_POINT_HH
# define MLN_MLC_SAME_POINT_HH

# include <mlc/equal.hh>
# include <core/macros.hh>


namespace mln
{

  namespace mlc
  {

    template <typename T1, typename T2>
    struct same_point : mlc::equal<oln_point(T2), oln_point(T2)>
    {
    };

  } // end of namespace mln::mlc

} // end of namespace mln


#endif // ! MLN_MLC_SAME_POINT_HH
