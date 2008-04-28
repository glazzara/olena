#ifndef MLN_LITERAL_BLACK_HH
# define MLN_LITERAL_BLACK_HH

/*! \file mln/literal/black.hh
 * \brief Definition of the literal of mln::black.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal black.
    struct black_t : public Literal<black_t>
    {
      T_rgb operator()() const
      {
	return value::rgb8(0,0,0);
      }
    };


    /// Literal black.
    // FIXME: Make this a proper global (issue #43).
    static const black_t black = black_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_BLACK_HH
