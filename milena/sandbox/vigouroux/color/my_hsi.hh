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
      hsi_(const H& hue, const S& sat, const I& i)
	: hue_(hue),
	  sat_(sat),
	  int_(i)
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
      const I& i() const
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
      I& i()
      {
	return this->int_;
      }

    private:
      H hue_;
      S sat_;
      I int_;
    };

    typedef hsi_<float, float, float> hsi_3x8;

    typedef hsi_<double, double, double> hsi_d;



//     template <unsigned n>
//     struct hsi
//     {
//     public:
//       /// Constructor without argument.
//       hsi<n>();

//       /// Constructor from component values.
//       hsi<n>(double hue, double sat, double i);

//       /// Access to component values.
//       double hue() const	{ return this->hue_; }
//       double sat() const	{ return this->sat_; }
//       double i() const	{ return this->int_; }

//       /// Set component values.
//       void hue(double hue)
//       {
// 	this->hue_ = hue;
//       }
//       void sat(double sat)
//       {
// 	this->sat_ = sat;
//       }
//       void i(double i)
//       {
// 	mln_precondition(i >= 0);
// 	this->int_ = i;
//       }

//     private:
//       double hue_;
//       double sat_;
//       double int_;
//     };


# ifndef MLN_INCLUDE_ONLY

//     template <unsigned n>
//     inline
//     hsi<n>::hsi()
//       :hue_(0), sat_(0), int_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     hsi<n>::hsi(double h, double sat, double i)
//     {
//       mln_invariant(h >= 0);
//       mln_invariant(sat >= 0);
//       mln_invariant(i >= 0);
//       this->hue_ = h;
//       this->sat_ = sat;
//       this->i_ = i;
//     }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSI_HH
