#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_CMY_HH
# define MLN_VALUE_CMY_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct CMY
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    CMY<E> >
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

      /// Read-write access to the y component.
      C& cyan()
      {
	return this->cyan_;
      }
      M& magenta()
      {
	return this->magenta_;
      }
      Y& yellow()
      {
	return this->yellow_;
      }

    private:
      C cyan_;
      M magenta_;
      Y yellow_;
    };

    typedef cmy_<float, float, float> cmy_f;

    typedef cmy_<double, double, double> cmy_d;
  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_CMY_HH
