#ifndef MLN_VALUE_XYZ_HH
# define MLN_VALUE_XYZ_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct XYZ
    {
    };

    template <typename X, typename Y, typename Z>
    class xyz_ : public XYZ< xyz_<X,Y,Z> >
    {
    public:

      typedef X x_type;
      typedef Y y_type;
      typedef Z z_type;

      /// Constructor without argument.
      xyz_()
      {
      }

      /// Constructor from component values.
      xyz_(const X& x, const Y& y, const Z& z)
	: x_(x),
	  y_(y),
	  z_(z)
      {
      }

      /// Read-only access to the x component.
      const X& x() const
      {
	return this->x_;
      }
      const Y& y() const
      {
	return this->y_;
      }
      const Z& z() const
      {
	return this->z_;
      }

      /// Read-write access to the x component.
      X& x()
      {
	return this->x_;
      }
      Y& y()
      {
	return this->y_;
      }
      Z& z()
      {
	return this->z_;
      }

    private:
      X x_;
      Y y_;
      Z z_;
    };

    typedef xyz_<float, float, float> xyz_3x8;

    typedef xyz_<double, double, double> xyz_d;

  } // end of namespace mln::value

} // end of namespace mln

//     template <unsigned n>
//     struct xyz
//     {
//     public:
//       /// Constructor without argument.
//       xyz<n>();

//       /// Constructor from component values.
//       xyz<n>(double x, double y, double z);

//       /// Access to component values.
//       double x() const	{ return this->x_; }
//       double y() const	{ return this->y_; }
//       double z() const	{ return this->z_; }

//       /// Set component values.
//       void x(double x)
//       {
// 	this->x_ = x;
//       }
//       void y(double y)
//       {
// 	this->y_ = y;
//       }
//       void z(double z)
//       {
// 	mln_precondition(z >= 0);
// 	this->z_ = z;
//       }

//     private:
//       double x_;
//       double y_;
//       double z_;
//     };

//     template <unsigned n>
//     inline
//     xyz<n>::xyz()
//       :x_(0), y_(0), z_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     xyz<n>::xyz(double x, double y, double z)
//     {
//       mln_precondition(z >= 0);
//       this->x_ = x;
//       this->y_ = y;
//       this->z_ = z;
//     }
//   }
// }

#endif // ! MLN_VALUE_XYZ_HH
