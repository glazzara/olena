#ifndef MLC_SAME_COORD_HH
# define MLC_SAME_COORD_HH

# include <mlc/equal.hh>
# include <mln/core/macros.hh>


namespace mln
{

  namespace mlc
  {

    template <typename T1, typename T2>
    struct same_coord : mlc::equal<mln_coord(T1), mln_coord(T2)>
    {
    };

  } // end of namespace mln::mlc

} // end of namespace mln


#endif // ! MLC_SAME_COORD_HH
