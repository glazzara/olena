#ifndef _ACCESS_HH
# define _ACCESS_HH

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/metal/is.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>
# include <mln/convert/to.hh>

namespace mln
{

  namespace interpolation
  {

    template < typename I >
    struct nearest_neighbor
      : public fun::internal::selector_<const algebra::vec<3,float>,
                                        // 3,float is a dummy parameter (real is n,T)
                                        mln_value(I), nearest_neighbor<I> >::ret
    {
      typedef mln_value(I) result;

      nearest_neighbor(const I& ima) : ima(ima) {}

      template < unsigned n, typename T >
      mln_value(I)
      operator()(const algebra::vec<n,T>& x) const
      {
        mln_psite(I) p =  convert::to<mln_psite(I)>(x);
        return ima(p);
      }

      const I& ima;
    };

    template < typename I >
    struct linear
      : public fun::internal::selector_<const algebra::vec<1,float>,
                                        // float is a dummy parameter (real is C)
                                        mln_value(I), linear<I> >::ret
    {
      typedef mln_value(I) result;

      linear(const I& ima) : ima(ima) {}

      template <typename C>
      mln_value(I)
      operator()(const algebra::vec<1,C>& v) const
      {
        typedef mln_sum(mln_value(I)) vsum;

        // looking for img(x);
        double x = v[0];

        // p1
        double xa = mln_point(I)::coord(v[0]);
        vsum ya = ima(point1d(xa));

        // x makes sens in img
        if (x == xa)
          return ima(xa);

        // p2
        double xb = mln_point(I)::coord(v[0] + 1);
        vsum yb = ima(point1d(xb));

        // Taylor-young
        return convert::to<mln_value(I)>
          (ya + (x - xa) * (yb - ya) / (xb - xa));
      }

      const I& ima;
    };

    template < typename I >
    struct bilinear
      : public fun::internal::selector_<const algebra::vec<3,float>,
                                        // 3,float is a dummy parameter (real is n,T)
                                        mln_value(I), linear<I> >::ret
    {
      typedef mln_value(I) result;

      bilinear(const I& ima) : ima(ima) {}

      template <unsigned n, typename T>
      mln_value(I)
      operator()(const algebra::vec<n,T>& v) const
      {
        typedef mln_sum(mln_value(I)) vsum;

        //  q12----r2----q22
        //   |      |     |
        //   |      x     |
        //   |      |     |
        //  q11----r1----q21

        // looking for img(P(x,y))
        double x = v[0];
        double y = v[1];

        double x1 = mln_psite(I)::coord(v[0]);
        double x2 = mln_psite(I)::coord(v[0]+ 1);
        double y1 = mln_psite(I)::coord(v[1]);
        double y2 = mln_psite(I)::coord(v[1]+ 1);

        vsum q11 = ima(point2d(x1, y1));
        vsum q12 = ima(point2d(x1, y2));
        vsum q21 = ima(point2d(x2, y1));
        vsum q22 = ima(point2d(x2, y2));

        //if (x2 - x1 == 0)
        //std::cout << x2 <<" - " << x1 << std::endl;
        double x2_x1 = (x2 - x1) ? x2 - x1 : 0.000001;
        double y2_y1 = (y2 - y1) ? y2 - y1 : 0.000001;

        // linear interpolation #1
        vsum img_r1 = q11 * (x2 - x) / (x2_x1) +
          q21 * (x - x1) / (x2_x1);
        //std::cout << "l1 :  "<< img_r1 << std::endl;

        // linear interpolation #2
        vsum img_r2 =  q12 * (x2 - x) / (x2_x1) +
          q22 * (x - x1) / (x2_x1);
        //std::cout << "l2 :  "<< img_r2 << std::endl;

        // interpolating in y direction
        // FIXME : Sometime try to cast neg value to rgb component
        vsum res = (img_r1 * (y2 - y) / (y2_y1)
                    + img_r2 * (y - y1) /(y2_y1));
        res[0] = (res[0] < 0) ? 0 : res[0];
        res[1] = (res[1] < 0) ? 0 : res[1];
        res[2] = (res[2] < 0) ? 0 : res[2];

        return convert::to<mln_value(I)>(res);
      }

      const I& ima;
    };
  }

  namespace access
  {

    template <typename I, typename T, typename F>
    mln_value(I)
    access(const I& img, const point2d& p,
           const T& trans, const F& interp)
    {
      mlc_is(typename T::invert,
             Bijection_x2x<typename T::invert>)::check();
      mlc_is(F, Function<F>)::check();

      return interp(img, (trans.inv())(p));
    }

  }

}

#endif /* _ACCESS_HH */

