// #include <mln/value/ops.hh>

// #include <mln/value/concept/vectorial.hh>
// #include <mln/value/int_u.hh>
// #include <mln/metal/vec.hh>

#ifndef MLN_VALUE_YUV_HH
# define MLN_VALUE_YUV_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct yuv
    {
    public:
      /// Constructor without argument.
      yuv<n>();
      
      /// Constructor from component values.
      yuv<n>(double y, double u, double v);
    
      /// Access to component values.
      double y() const	{ return this->y_; }
      double u() const	{ return this->u_; }
      double v() const	{ return this->v_; }
      
      /// Set component values.
      void y(double y)	
      { 
	this->y_ = y;
      }
      void u(double u)	
      { 
	this->u_ = u;
      }
      void v(double v)	
      { 
	mln_precondition(v >= 0);
	this->v_ = v;
      }

    private:
      double y_;
      double u_;
      double v_;
    };

    template <unsigned n>
    inline
    yuv<n>::yuv()
      :y_(0), u_(0), v_(0)
    {
    }

    template <unsigned n>
    inline
    yuv<n>::yuv(double y, double u, double v)
    {
      mln_precondition(v >= 0);
      this->y_ = y;
      this->u_ = u;
      this->v_ = v;
    }
  }
}

#endif // ! MLN_VALUE_YUV_HH
