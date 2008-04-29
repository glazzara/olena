#ifndef MLN_LITERAL_WHITE_HH
# define MLN_LITERAL_WHITE_HH

/*! \file mln/literal/white.hh
 * \brief Definition of the literal of mln::white.
 *
 */

# include <mln/core/concept/literal.hh>
# include <mln/value/rgb8.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal white.
    struct white_t : public Literal<white_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(255,255,255);
      }
    };


    /// Literal white.
    // FIXME: Make this a proper global (issue #43).
    static const white_t white = white_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_WHITE_HH
