#ifndef MLN_LITERAL_MAGENTA_HH
# define MLN_LITERAL_MAGENTA_HH

/*! \file mln/literal/magenta.hh
 * \brief Definition of the literal of mln::magenta.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal magenta.
    struct magenta_t : public Literal<magenta_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(255, 0, 255);
      }
    };


    /// Literal magenta.
    // FIXME: Make this a proper global (issue #43).
    static const magenta_t magenta = magenta_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_MAGENTA_HH
