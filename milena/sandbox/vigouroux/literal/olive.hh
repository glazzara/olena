#ifndef MLN_LITERAL_OLIVE_HH
# define MLN_LITERAL_OLIVE_HH

/*! \file mln/literal/olive.hh
 * \brief Definition of the literal of mln::olive.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal olive.
    struct olive_t : public Literal<olive_t>
    {
    };


    /// Literal olive.
    // FIXME: Make this a proper global (issue #43).
    static const olive_t olive = olive_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_OLIVE_HH
