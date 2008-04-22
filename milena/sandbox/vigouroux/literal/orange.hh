#ifndef MLN_LITERAL_ORANGE_HH
# define MLN_LITERAL_ORANGE_HH

/*! \file mln/literal/orange.hh
 * \brief Definition of the literal of mln::orange.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal orange.
    struct orange_t : public Literal<orange_t>
    {
    };


    /// Literal orange.
    // FIXME: Make this a proper global (issue #43).
    static const orange_t orange = orange_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_ORANGE_HH
