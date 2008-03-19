#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_CMY_HH
# define MLN_VALUE_CMY_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct CMY
    {
    };

    template <typename C, typename M, typename Y>
    class cmy_ : public CMY< cmy_<C,M,Y> >
    {
    public:

      typedef C c_type;
      typedef M m_type;
      typedef Y y_type;

      /// Constructor without argument.
      cmy_()
      {
      }

      /// Constructor from component values.
      cmy_(const C& cyan, const M& magenta, const Y& yellow)
	: cyan_(cyan),
	  magenta_(magenta),
	  yellow_(yellow)
      {
      }

      /// Read-only access to the cyan component.
      const C& cyan() const
      {
	return this->cyan_;
      }
      const M& magenta() const
      {
	return this->magenta_;
      }
      const Y& yellow() const
      {
	return this->yellow_;
      }

    private:
      C cyan_;
      M magenta_;
      Y yellow_;
    };

    typedef cmy_<float, float, float> cmy_3x8;

    typedef cmy_<double, double, double> cmy_d;
  } // end of namespace mln::value

} // end of namespace mln

//     template <unsigned n>
//     struct cmy
//     {
//     public:
//       /// Constructor without argument.
//       cmy();

//       /// Constructor from component values.
//       cmy(double c, double m, double y);

//       /// Access to component values.
//       double c() const	{ return this->c_; }
//       double m() const	{ return this->m_; }
//       double y() const	{ return this->y_; }

//       /// Set component values.
//       void c(double c)
//       {
// 	mln_precondition(c >= 0);
// 	this->c_ = c;
//       }
//       void m(double m)
//       {
// 	mln_precondition(m >= 0);
// 	this->m_ = m;
//       }
//       void y(double y)
//       {
// 	mln_precondition(y >= 0);
// 	this->y_ = y;
//       }

//     private:
//       double c_;
//       double m_;
//       double y_;
//     };

//     template <unsigned n>
//     inline
//     cmy<n>::cmy()
//       :c_(0), m_(0), y_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     cmy<n>::cmy(double c, double m, double y)
//     {
//       mln_precondition(c >= 0);
//       mln_precondition(m >= 0);
//       mln_precondition(y >= 0);
//       this->c_ = c;
//       this->m_ = m;
//       this->y_ = y;
//     }

#endif // ! MLN_VALUE_CMY_HH
