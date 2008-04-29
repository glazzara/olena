#ifndef MLN_LITERAL_CYAN_HH
# define MLN_LITERAL_CYAN_HH

/*! \file mln/literal/cyan.hh
 * \brief Definition of the literal of mln::cyan.
 *
 */

# include <mln/core/concept/literal.hh>
# include <mln/value/rgb8.hh>
# include "../cmy/my_cmy.hh"
# include "../cmy/rgb_to_cmy.hh"

namespace mln
{

  namespace literal
  {

    /// Type of literal cyan.
    struct cyan_t : public Literal<cyan_t>
    {
      value::rgb8 operator()() const
      {
	const value::cmy_f cmy(255,0,0);
	value::rgb8 rgb;
	rgb = fun::v2v::f_cmy_to_rgb_3x8_t(cmy);

	return rgb;
      }
    };


    /// Literal cyan.
    // FIXME: Make this a proper global (issue #43).
    static const cyan_t cyan = cyan_t();


  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_CYAN_HH
