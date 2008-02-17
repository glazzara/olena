#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/metal/vec.hh>

#ifndef MLN_VALUE_HSV_HH
# define MLN_VALUE_HSV_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct hsv
    {
    public:
      /// Constructor without argument.
      hsv<n>();
      
      /// Constructor from component values.
      hsv<n>(int h, int s, int v);

      /// Access to component values.
      double h() const	{ return this->h_; }
      double s() const	{ return this->s_; }
      double v() const	{ return this->v_; }

      /// Set component values.
      void h(double h)	
      { 
	this->h_ = h;
      }
      void s(double s)	
      { 
	this->s_ = s;
      }
      void v(double v)	
      { 
	mln_precondition(v >= 0);
	this->v_ = v;
      }
    
    private:
      double h_;
      double s_;
      double v_;
    };

    template <unsigned n>
    inline
    hsi<n>::hsv()
      :h_(0), s_(0), v_(0)
    {
    }

    template <unsigned n>
    inline
    hsv<n>::hsv(int h, int s, int v)
    {
      mln_precondition(h >= 0);
      mln_precondition(s >= 0);
      mln_precondition(v >= 0);
      this->h_ = h;
      this->s_ = s;
      this->v_ = v;
    }
  }
}

#endif // ! MLN_VALUE_HSV_HH
