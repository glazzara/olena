#ifndef MLN_VALUE_YUV_HH
# define MLN_VALUE_YUV_HH

namespace mln
{
  namespace value
  {
//     template <unsigned n>
    struct cmy
    {
    public:
      /// Constructor without argument.
      cmy();
      
      /// Constructor from component values.
      cmy(double c, double m, double y);
    
      /// Access to component values.
      double c() const	{ return this->c_; }
      double m() const	{ return this->m_; }
      double y() const	{ return this->y_; }
      
      /// Set component values.
      void c(double c)	
      { 
	mln_precondition(c >= 0);
	this->c_ = c;
      }
      void m(double m)	
      { 
	mln_precondition(m >= 0);
	this->m_ = m;
      }
      void y(double y)	
      { 
	mln_precondition(y >= 0);
	this->y_ = y;
      }

    private:
      double c_;
      double m_;
      double y_;
    };

//     template <unsigned n>
    inline
    yuv::cmy()
      :c_(0), m_(0), y_(0)
    {
    }

//     template <unsigned n>
    inline
    cmy::cmy(double c, double m, double y)
    {
      mln_precondition(c >= 0);
      mln_precondition(m >= 0);
      mln_precondition(y >= 0);
      this->c_ = c;
      this->m_ = m;
      this->y_ = y;
    }
  }
}

#endif // ! MLN_VALUE_YUV_HH
