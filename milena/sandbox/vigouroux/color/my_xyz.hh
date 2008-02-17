// #include <mln/value/ops.hh>

// #include <mln/value/concept/vectorial.hh>
// #include <mln/value/int_u.hh>
// #include <mln/metal/vec.hh>

#ifndef MLN_VALUE_XYZ_HH
# define MLN_VALUE_XYZ_HH

namespace mln
{
  namespace value
  {
    template <unsigned n>
    struct xyz
    {
    public:
      /// Constructor without argument.
      xyz<n>();
      
      /// Constructor from component values.
      xyz<n>(double x, double y, double z);
    
      /// Access to component values.
      double x() const	{ return this->x_; }
      double y() const	{ return this->y_; }
      double z() const	{ return this->z_; }
      
      /// Set component values.
      void x(double x)	
      { 
	this->x_ = x;
      }
      void y(double y)	
      { 
	this->y_ = y;
      }
      void z(double z)	
      { 
	mln_precondition(z >= 0);
	this->z_ = z;
      }

    private:
      double x_;
      double y_;
      double z_;
    };

    template <unsigned n>
    inline
    xyz<n>::xyz()
      :x_(0), y_(0), z_(0)
    {
    }

    template <unsigned n>
    inline
    xyz<n>::xyz(double x, double y, double z)
    {
      mln_precondition(z >= 0);
      this->x_ = x;
      this->y_ = y;
      this->z_ = z;
    }
  }
}

#endif // ! MLN_VALUE_XYZ_HH
