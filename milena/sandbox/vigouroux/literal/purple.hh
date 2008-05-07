#ifndef MLN_LITERAL_PURPLE_HH
# define MLN_LITERAL_PURPLE_HH

/*! \file mln/literal/purple.hh
 * \brief Definition of the literal of mln::purple.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal purple.
    struct purple_t : public Literal<purple_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(128,0,128);
      }
    };


    /// Literal purple.
    // FIXME: Make this a proper global (issue #43).
    static const purple_t purple = purple_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_PURPLE_HH
