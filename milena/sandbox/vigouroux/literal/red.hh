#ifndef MLN_LITERAL_RED_HH
# define MLN_LITERAL_RED_HH

/*! \file mln/literal/red.hh
 * \brief Definition of the literal of mln::red.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal red.
    struct red_t : public Literal<red_t>
    {
      T_rgb operator()() const
      {
	return value::rgb8(255,0,0);
      }
    };


    /// Literal red.
    // FIXME: Make this a proper global (issue #43).
    static const red_t red = red_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_RED_HH
