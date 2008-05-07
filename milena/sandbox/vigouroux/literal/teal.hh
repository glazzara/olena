#ifndef MLN_LITERAL_TEAL_HH
# define MLN_LITERAL_TEAL_HH

/*! \file mln/literal/teal.hh
 * \brief Definition of the literal of mln::teal.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal teal.
    struct teal_t : public Literal<teal_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(0,128,128);
      }
    };


    /// Literal teal.
    // FIXME: Make this a proper global (issue #43).
    static const teal_t teal = teal_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_TEAL_HH
