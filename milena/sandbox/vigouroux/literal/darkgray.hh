#ifndef MLN_LITERAL_DARKGRAY_HH
# define MLN_LITERAL_DARKGRAY_HH

/*! \file mln/literal/darkgray.hh
 * \brief Definition of the literal of mln::darkgray.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal darkgray.
    struct darkgray_t : public Literal<darkgray_t>
    {
    };


    /// Literal darkgray.
    // FIXME: Make this a proper global (issue #43).
    static const darkgray_t darkgray = darkgray_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_DARKGRAY_HH
