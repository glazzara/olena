#ifndef MLN_LITERAL_YELLOW_HH
# define MLN_LITERAL_YELLOW_HH

/*! \file mln/literal/yellow.hh
 * \brief Definition of the literal of mln::yellow.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal yellow.
    struct yellow_t : public Literal<yellow_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(255,255,0);
      }
    };


    /// Literal yellow.
    // FIXME: Make this a proper global (issue #43).
    static const yellow_t yellow = yellow_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_YELLOW_HH
