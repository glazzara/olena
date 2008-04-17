#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#ifndef MLN_VALUE_HSL_HH
# define MLN_VALUE_HSL_HH

namespace mln
{

  namespace value
  {

    template <typename E>
    struct HSL
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    HSL<E> >
    {
    };

    template <typename H, typename S, typename L>
    class hsl_ : public HSL< hsl_<H,S,L> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef L l_type;

      /// Constructor without argument.
      hsl_()
      {
      }

      /// Constructor from component values.
      hsl_(const H& hue, const S& sat, const L& light)
	: hue_(hue),
	  sat_(sat),
	  light_(light)
      {
      }

      /// Read-only access to the hue component.
      const H& hue() const
      {
	return this->hue_;
      }
      const S& sat() const
      {
	return this->sat_;
      }
      const L& light() const
      {
	return this->light_;
      }

    /// Read-write access to the hue component.
      H& hue()
      {
	return this->hue_;
      }
      S& sat()
      {
	return this->sat_;
      }
      L& light()
      {
	return this->light_;
      }

    private:
      H hue_;
      S sat_;
      L light_;
    };

    typedef hsl_<float, float, float> hsl_f;

    typedef hsl_<double, double, double> hsl_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSL_HH
