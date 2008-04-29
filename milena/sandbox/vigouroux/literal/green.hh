#ifndef MLN_LITERAL_GREEN_HH
# define MLN_LITERAL_GREEN_HH

/*! \file mln/literal/green.hh
 * \brief Definition of the literal of mln::green.
 *
 */

# include <mln/core/concept/literal.hh>
# include <mln/value/rgb8.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal green.
    struct green_t : public Literal<green_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(0,255,0);
      }
    };


    /// Literal green.
    // FIXME: Make this a proper global (issue #43).
    static const green_t green = green_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_GREEN_HH
