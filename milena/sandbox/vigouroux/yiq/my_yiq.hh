#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_YIQ_HH
# define MLN_VALUE_YIQ_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct YIQ
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    YIQ<E> >
    {
    };

    template <typename Y, typename I, typename Q>
    class yiq_ : public YIQ< yiq_<Y,I,Q> >
    {
    public:

      typedef Y y_type;
      typedef I i_type;
      typedef Q q_type;

      /// Constructor without argument.
      yiq_()
      {
      }

      /// Constructor from component values.
      yiq_(const Y& y, const I& i, const Q& q)
	: y_(y),
	  i_(i),
	  q_(q)
      {
      }

      /// Read-only access to the y component.
      const Y& y() const
      {
	return this->y_;
      }
      const I& i() const
      {
	return this->i_;
      }
      const Q& q() const
      {
	return this->q_;
      }

      /// Read-write access to the y component.
      Y& y()
      {
	return this->y_;
      }
      I& i()
      {
	return this->i_;
      }
      Q& q()
      {
	return this->q_;
      }

    private:
      Y y_;
      I i_;
      Q q_;
    };

    typedef yiq_<float, float, float> yiq_f;

    typedef yiq_<double, double, double> yiq_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_YIQ_HH
