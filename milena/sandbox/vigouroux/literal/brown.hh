#ifndef MLN_LITERAL_BROWN_HH
# define MLN_LITERAL_BROWN_HH

/*! \file mln/literal/brown.hh
 * \brief Definition of the literal of mln::brown.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal brown.
    struct brown_t : public Literal<brown_t>
    {
      value::rgb8 operator()() const
      {
	return value::rgb8(150,75,0);
      }
    };


    /// Literal brown.
    // FIXME: Make this a proper global (issue #43).
    static const brown_t brown = brown_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_BROWN_HH
