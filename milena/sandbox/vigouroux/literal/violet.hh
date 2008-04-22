#ifndef MLN_LITERAL_VIOLET_HH
# define MLN_LITERAL_VIOLET_HH

/*! \file mln/literal/violet.hh
 * \brief Definition of the literal of mln::violet.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal violet.
    struct violet_t : public Literal<violet_t>
    {
    };


    /// Literal violet.
    // FIXME: Make this a proper global (issue #43).
    static const violet_t violet = violet_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_VIOLET_HH
