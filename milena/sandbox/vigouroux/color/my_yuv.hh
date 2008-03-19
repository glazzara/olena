#ifndef MLN_VALUE_YUV_HH
# define MLN_VALUE_YUV_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct YUV
    {
    };

    template <typename Y, typename U, typename V>
    class yuv_ : public YUV< yuv_<Y,U,V> >
    {
    public:

      typedef Y y_type;
      typedef U u_type;
      typedef V v_type;

      /// Constructor without argument.
      yuv_()
      {
      }

      /// Constructor from component values.
      yuv_(const Y& y, const U& u, const V& v)
	: y_(y),
	  u_(u),
	  v_(v)
      {
      }

      /// Read-only access to the y component.
      const Y& y() const
      {
	return this->y_;
      }
      const U& u() const
      {
	return this->u_;
      }
      const V& v() const
      {
	return this->v_;
      }

      /// Read-write access to the y component.
      Y& y()
      {
	return this->y_;
      }
      U& u()
      {
	return this->u_;
      }
      V& v()
      {
	return this->v_;
      }

    private:
      Y y_;
      U u_;
      V v_;
    };

    typedef yuv_<float, float, float> yuv_3x8;

    typedef yuv_<double, double, double> yuv_d;

  } // end of namespace mln::value

} // end of namespace mln

//     template <unsigned n>
//     struct yuv
//     {
//     public:
//       /// Constructor without argument.
//       yuv<n>();

//       /// Constructor from component values.
//       yuv<n>(double y, double u, double v);

//       /// Access to component values.
//       double y() const	{ return this->y_; }
//       double u() const	{ return this->u_; }
//       double v() const	{ return this->v_; }

//       /// Set component values.
//       void y(double y)
//       {
// 	this->y_ = y;
//       }
//       void u(double u)
//       {
// 	this->u_ = u;
//       }
//       void v(double v)
//       {
// 	mln_precondition(v >= 0);
// 	this->v_ = v;
//       }

//     private:
//       double y_;
//       double u_;
//       double v_;
//     };

//     template <unsigned n>
//     inline
//     yuv<n>::yuv()
//       :y_(0), u_(0), v_(0)
//     {
//     }

//     template <unsigned n>
//     inline
//     yuv<n>::yuv(double y, double u, double v)
//     {
//       mln_precondition(v >= 0);
//       this->y_ = y;
//       this->u_ = u;
//       this->v_ = v;
//     }
//   }
// }

#endif // ! MLN_VALUE_YUV_HH
