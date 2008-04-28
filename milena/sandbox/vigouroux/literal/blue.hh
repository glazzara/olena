#ifndef MLN_LITERAL_BLUE_HH
# define MLN_LITERAL_BLUE_HH

/*! \file mln/literal/blue.hh
 * \brief Definition of the literal of mln::blue.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal blue.
    struct blue_t : public Literal<blue_t>
    {
      T_rgb operator()() const
      {
	return value::rgb8(0,0,255);
      }
    };


    /// Literal blue.
    // FIXME: Make this a proper global (issue #43).
    static const blue_t blue = blue_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_BLUE_HH
