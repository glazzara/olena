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

    typedef hsv_<float, float, float> hsv_3x8;

    typedef hsv_<double, double, double> hsv_d;

  } // end of namespace mln::value

} // end of namespace mln

//     template <unsigned n>
//     struct hsv
//     {
//     public:
//       /// Constructor without argument.
//       hsv<n>();

//       /// Constructor from component values.
//       hsv<n>(int h, int s, int v);

//       /// Access to component values.
//       double h() const	{ return this->h_; }
//       double s() const	{ return this->s_; }
//       double v() const	{ return this->v_; }

//       /// Set component values.
//       void h(double h)
//       {
// 	this->h_ = h;
//       }
//       void s(double s)
//       {
// 	this->s_ = s;
//       }
//       void v(double v)
//       {
// 	mln_precondition(v >= 0);
// 	this->v_ = v;
//       }

//     private:
//       double h_;
//       double s_;
//       double v_;
//     };

//     template <unsigned n>
//     inline
//     hsi<n>::hsv()
//       :h_(0), s_(0), v_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     hsv<n>::hsv(int h, int s, int v)
//     {
//       mln_precondition(h >= 0);
//       mln_precondition(s >= 0);
//       mln_precondition(v >= 0);
//       this->h_ = h;
//       this->s_ = s;
//       this->v_ = v;
//     }
//   }
// }

#endif // ! MLN_VALUE_HSV_HH
