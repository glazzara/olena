#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#ifndef MLN_VALUE_HSV_HH
# define MLN_VALUE_HSV_HH

namespace mln
{

  namespace value
  {

    template <typename E>
    struct HSV
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    HSV<E> >
    {
    };

    template <typename H, typename S, typename V>
    class hsv_ : public HSV< hsv_<H,S,V> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef V v_type;

      /// Constructor without argument.
      hsv_()
      {
      }

      /// Constructor from component values.
      hsv_(const H& hue, const S& sat, const V& value)
	: hue_(hue),
	  sat_(sat),
	  value_(value)
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
      const V& value() const
      {
	return this->value_;
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
      V& value()
      {
	return this->value_;
      }

    private:
      H hue_;
      S sat_;
      V value_;
    };

    typedef hsv_<float, float, float> hsv_f;

    typedef hsv_<double, double, double> hsv_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSV_HH
