#ifndef MLN_LITERAL_LIGHTGRAY_HH
# define MLN_LITERAL_LIGHTGRAY_HH

/*! \file mln/literal/lightgray.hh
 * \brief Definition of the literal of mln::lightgray.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal lightgray.
    struct lightgray_t : public Literal<lightgray_t>
    {
    };


    /// Literal lightgray.
    // FIXME: Make this a proper global (issue #43).
    static const lightgray_t lightgray = lightgray_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_LIGHTGRAY_HH
