#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/metal/vec.hh>

#ifndef MLN_VALUE_HSL_HH
# define MLN_VALUE_HSL_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct hsl
    {
    public:
      /// Constructor without argument.
      hsl<n>();
      
      /// Constructor from component values.
      hsl<n>(int h, int s, int l);

      /// Access to component values.
      double h() const	{ return this->h_; }
      double s() const	{ return this->s_; }
      double l() const	{ return this->l_; }

      /// Set component values.
      void h(double h)	
      { 
	this->h_ = h;
      }
      void s(double s)	
      { 
	this->s_ = s;
      }
      void l(double l)	
      { 
	mln_precondition(l >= 0);
	this->l_ = l;
      }
    
    private:
      double h_;
      double s_;
      double l_;
    };

    template <unsigned n>
    inline
    hsl<n>::hsl()
      :h_(0), s_(0), l_(0)
    {
    }

    template <unsigned n>
    inline
    hsl<n>::hsl(int h, int s, int l)
    {
      mln_precondition(h >= 0);
      mln_precondition(s >= 0);
      mln_precondition(l >= 0);
      this->h_ = h;
      this->s_ = s;
      this->l_ = l;
    }
  }
}

#endif // ! MLN_VALUE_HSL_HH
