#ifndef MLN_LITERAL_PINK_HH
# define MLN_LITERAL_PINK_HH

/*! \file mln/literal/pink.hh
 * \brief Definition of the literal of mln::pink.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal pink.
    struct pink_t : public Literal<pink_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(250,218,221);
      }
    };


    /// Literal pink.
    // FIXME: Make this a proper global (issue #43).
    static const pink_t pink = pink_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_PINK_HH
