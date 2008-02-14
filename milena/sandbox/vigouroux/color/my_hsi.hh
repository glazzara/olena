#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/metal/vec.hh>

#ifndef MLN_VALUE_HSI_HH
# define MLN_VALUE_HSI_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct hsi
    {
    public:
      /// Constructor without argument.
      hsi<n>();
      
      /// Constructor from component values.
      hsi<n>(int h, int s, int i);

      /// Access to component values.
      double h() const	{ return this->h_; }
      double s() const	{ return this->s_; }
      double i() const	{ return this->i_; }

      /// Set component values.
      void h(double h)	
      { 
	this->h_ = h;
      }
      void s(double s)	
      { 
	this->s_ = s;
      }
      void i(double i)	
      { 
	mln_precondition(i >= 0);
	this->i_ = i;
      }
    
    private:
      double h_;
      double s_;
      double i_;
    };

    template <unsigned n>
    inline
    hsi<n>::hsi()
      :h_(0), s_(0), i_(0)
    {
    }

    template <unsigned n>
    inline
    hsi<n>::hsi(int h, int s, int i)
    {
      mln_precondition(h >= 0);
      mln_precondition(s >= 0);
      mln_precondition(i >= 0);
      this->h_ = h;
      this->s_ = s;
      this->i_ = i;
    }
  }
}

#endif // ! MLN_VALUE_HSI_HH
