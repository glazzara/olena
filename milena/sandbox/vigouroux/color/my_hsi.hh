#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/metal/vec.hh>

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

    template <typename F>
    class hsi_ : public HSI< hsi_<F> >
    {
    public:

      typedef F h_type;
      typedef F s_type;
      typedef F i_type;

      /// Constructor without argument.
      hsi_()
      {
      }
      
      /// Constructor from component values.
      hsi_(const F& h, const F& s, const F& i)
	: h_(h),
	  s_(s),
	  i_(i)
      {
      }

      /// Read-only access to the h component.
      const F& h() const
      {
	return this->h_;
      }
      const F& s() const
      {
	return this->s_;
      }
      const F& i() const
      {
	return this->i_;
      }

      /// Read-write access to the h component.
      F& h()	
      { 
	return this->h_;
      }
      F& s()
      {
	return this->s_;
      }
      F& i()
      {
	return this->i_;
      }
    
    private:
      F h_;
      F s_;
      F i_;
    };

    typedef hsi_<float01_8> hsi_3x8;

    typedef hsi_<double> hsi_d;



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


# ifndef MLN_INCLUDE_ONLY

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

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_HSI_HH
