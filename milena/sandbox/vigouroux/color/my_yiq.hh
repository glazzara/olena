// #include <mln/value/ops.hh>

// #include <mln/value/concept/vectorial.hh>
// #include <mln/value/int_u.hh>
// #include <mln/metal/vec.hh>

#ifndef MLN_VALUE_YIQ_HH
# define MLN_VALUE_YIQ_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct yiq
    {
    public:
      /// Constructor without argument.
      yiq<n>();
      
      /// Constructor from component values.
      yiq<n>(double y, double i, double q);
    
      /// Access to component values.
      double y() const	{ return this->y_; }
      double i() const	{ return this->i_; }
      double q() const	{ return this->q_; }
      
      /// Set component values.
      void y(double y)	
      { 
	this->y_ = y;
      }
      void i(double i)	
      { 
	this->i_ = i;
      }
      void q(double q)	
      { 
	mln_precondition(q >= 0);
	this->q_ = q;
      }

    private:
      double y_;
      double i_;
      double q_;
    };

    template <unsigned n>
    inline
    yiq<n>::yiq()
      :y_(0), i_(0), q_(0)
    {
    }

    template <unsigned n>
    inline
    yiq<n>::yiq(double y, double i, double q)
    {
      mln_precondition(q >= 0);
      this->y_ = y;
      this->i_ = i;
      this->q_ = q;
    }
  }
}

#endif // ! MLN_VALUE_YIQ_HH
