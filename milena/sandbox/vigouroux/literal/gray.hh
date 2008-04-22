#ifndef MLN_LITERAL_GRAY_HH
# define MLN_LITERAL_GRAY_HH

/*! \file mln/literal/gray.hh
 * \brief Definition of the literal of mln::gray.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal gray.
    struct gray_t : public Literal<gray_t>
    {
    };


    /// Literal gray.
    // FIXME: Make this a proper global (issue #43).
    static const gray_t gray = gray_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_GRAY_HH
