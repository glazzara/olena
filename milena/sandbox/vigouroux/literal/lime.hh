#ifndef MLN_LITERAL_LIME_HH
# define MLN_LITERAL_LIME_HH

/*! \file mln/literal/lime.hh
 * \brief Definition of the literal of mln::lime.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal lime.
    struct lime_t : public Literal<lime_t>
    {
    };


    /// Literal lime.
    // FIXME: Make this a proper global (issue #43).
    static const lime_t lime = lime_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_LIME_HH
