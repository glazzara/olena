#ifndef _ACCESS_HH
# define _ACCESS_HH

# include <mln/core/image1d.hh>
# include <mln/core/image2d.hh>
# include <mln/metal/is.hh>
# include <mln/core/concept/function.hh>

namespace mln
{

  namespace interpolation
  {

    template < typename I >
    struct nearest_neighbor
      : public Function_x2x< nearest_neighbor<I> >
    {
      typedef mln_value(I) result;
      
      template < typename V >
      mln_value(I)
      operator()(const I& img, const V& v) const
      {
        mln_point(I) p =  algebra::to_point<mln_point(I)>(v);
        return img(p);
      }

    };
    
    template < typename I >
    struct linear
      : public Function_x2x< linear<I> >
    {
      typedef mln_value(I) result;
      
      template <typename C>
      mln_value(I)
      operator()(const I& img,
                 const algebra::vec<1,C>& v) const
      {
        // looking for img(x);
        double x = v[0];

        // p1
        double xa = mln_point(I)::coord(v[0]);
        double ya = img(point1d(xa));

        // x makes sens in img
        if (x == xa)
          return img(xa);

        // p2
        double xb = mln_point(I)::coord(v[0] + 1);
        double yb = img(point1d(xb));

        // Taylor-young
        return ya + (x - xa) * (yb - ya) / (xb - xa);
      }
    };

    template < typename I >
    struct bilinear
      : public Function_x2x< bilinear<I> >
    {
      typedef mln_value(I) result;
      
      template <typename V>
      mln_value(I)
      operator()(const I& img, const V& v) const
      {
        //  q12----r2----q22
        //   |      |     |
        //   |      x     |
        //   |      |     |
        //  q11----r1----q21

        // looking for img(P(x,y))
        double x = v[0];
        double y = v[1];
        
        double x1 = mln_point(I)::coord(v[0]);
        double x2 = mln_point(I)::coord(v[0]+ 1);
        double y1 = mln_point(I)::coord(v[1]);
        double y2 = mln_point(I)::coord(v[1]+ 1);

        point2d q11 = point2d(x1, y1);
        point2d q12 = point2d(x1, y2);
        point2d q21 = point2d(x2, y1);
        point2d q22 = point2d(x2, y2);
        
        // linear interpolation #1
        mln_value(I) img_r1 = img(q11) * (x2 - x) / (x2 - x1) +
          img(q21) * (x - x1) / (x2 - x1);
        
        // linear interpolation #2
        mln_value(I) img_r2 =  img(q12) * (x2 - x) / (x2 - x1) +
          img(q22) * (x - x1) / (x2 - x1);

        // interpolating in y direction
        return img_r1 * (y2 - y) / (y2 -y1)
          + img_r2 * (y - y1) /(y2 - y1);
      }
    };
  }
  
  namespace access
  {

    template <typename I, typename T, typename F>
    mln_value(I)
    access(const I& img, const mln_point(I)& p,
           const T& trans, const F& interp)
    {
      mlc_is(typename T::invert, Bijection_x2x<typename T::invert>)::check();
      mlc_is(F, Function_x2x<F>)::check();

      return interp(img, (trans.inv())(p));
    }

  }

}

#endif /* _ACCESS_HH */

