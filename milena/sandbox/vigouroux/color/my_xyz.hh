#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_XYZ_HH
# define MLN_VALUE_XYZ_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct XYZ
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    XYZ<E> >
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



#endif // ! MLN_VALUE_XYZ_HH
