#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_HSI_HH
# define MLN_VALUE_HSI_HH


namespace mln
{

  namespace value
  {

    template <typename E>
    struct HSI
    {
    };

    template <typename H, typename S, typename I>
    class hsi_ : public HSI< hsi_<H,S,I> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef I i_type;

      /// Constructor without argument.
      hsi_()
      {
      }

      /// Constructor from component values.
      hsi_(const H& hue, const S& sat, const I& inty)
	: hue_(hue),
	  sat_(sat),
	  int_(inty)
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
      const I& inty() const
      {
	return this->int_;
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
      I& inty()
      {
	return this->int_;
      }

    private:
      H hue_;
      S sat_;
      I int_;
    };

    typedef hsi_<float, float, float> hsi_f;

    typedef hsi_<double, double, double> hsi_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSI_HH
