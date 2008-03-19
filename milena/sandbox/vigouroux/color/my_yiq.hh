#ifndef MLN_VALUE_YIQ_HH
# define MLN_VALUE_YIQ_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct YIQ
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

    typedef yiq_<float, float, float> yiq_3x8;

    typedef yiq_<double, double, double> yiq_d;

  } // end of namespace mln::value

} // end of namespace mln

//     template <unsigned n>
//     struct yiq
//     {
//     public:
//       /// Constructor without argument.
//       yiq<n>();

//       /// Constructor from component values.
//       yiq<n>(double y, double i, double q);

//       /// Access to component values.
//       double y() const	{ return this->y_; }
//       double i() const	{ return this->i_; }
//       double q() const	{ return this->q_; }

//       /// Set component values.
//       void y(double y)
//       {
// 	this->y_ = y;
//       }
//       void i(double i)
//       {
// 	this->i_ = i;
//       }
//       void q(double q)
//       {
// 	mln_precondition(q >= 0);
// 	this->q_ = q;
//       }

//     private:
//       double y_;
//       double i_;
//       double q_;
//     };

//     template <unsigned n>
//     inline
//     yiq<n>::yiq()
//       :y_(0), i_(0), q_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     yiq<n>::yiq(double y, double i, double q)
//     {
//       mln_precondition(q >= 0);
//       this->y_ = y;
//       this->i_ = i;
//       this->q_ = q;
//     }
//   }
// }

#endif // ! MLN_VALUE_YIQ_HH
