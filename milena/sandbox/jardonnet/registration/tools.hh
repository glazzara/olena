#ifndef REGISTRATION_TOOL_HH
# define REGISTRATION_TOOL_HH

// temporary

# include <mln/algebra/mat.hh>
# include <mln/core/site_set/p_array.hh>

//# include "quat7.hh"


namespace mln
{

  template <typename P>
  void shuffle(p_array<P>& a)
  {
    for (unsigned int i = 0; i < a.nsites(); i++)
      {
        unsigned int r = rand() % a.nsites();
        P tmp;
        tmp = a[i];
        a[i] = a[r];
        a[r] = tmp;
      }
  }

  template <unsigned int n, typename T>
  struct buffer
  {
    buffer()
      : setted(0)
    {
    }

    void store(T e)
    {
      for (unsigned i = 0; i < n-1; i++)
        buf[i+1] = buf[i];
      buf[0] = e;

      setted++;
    }

    T& operator[](unsigned int i)
    {
      assert(i < n && i < setted);
      return buf[i];
    }

    const T * get_array()
    {
      return buf;
    }

  private:
    T buf[n];
    unsigned int setted;
  };

  //FIXME: groe length
  template <typename P>
  struct closest_point
  {
    typedef algebra::vec<P::dim, float> input;
    typedef P result;

    closest_point(const p_array<P>& X, const box<P>& box)
      : X(X), box(box)
#ifndef NDEBUG
      , i(0)
#endif
    { }

    result
    //inline
    operator () (const input& Ck) const
    {

#ifndef NDEBUG
      ++i;
#endif

      algebra::vec<P::dim,float> Cki = Ck;
      algebra::vec<P::dim,float> best_x = X[0];
      float best_d = norm::l2(Cki - best_x);
      for (size_t j = 1; j < X.nsites(); ++j)
        {
          algebra::vec<P::dim,float> Xj = X[j];
          float d = norm::l2(Cki - Xj);
          if (d < best_d)
            {
              best_d = d;
              best_x = Xj;
            }
        }
      return algebra::to_point<P>(best_x);
    }

    const box<P>& domain() const
    {
      return box;
    }

    const p_array<P>& X;
    const box<P>     box;

#ifndef NDEBUG
    mutable unsigned i;
#endif
  };

  /*
  // FIXME: Should be a morpher ?
  // we could acces domain of a lazy map, iterator etc...
  template < typename F >
  struct lazy_image
  {
    // Fun is potentially an image.
    lazy_image(const F& fun)
      : value(fun.domain()), is_known(fun.domain()), fun(fun)
    {
    }

    // FIXME: hack, remove this constructor
    lazy_image(const F& fun, int nrows, int ncols, int nslis)
      : value(nrows, ncols,1), is_known(nrows,ncols,1), fun(fun)
    { }

    const mln_result(F)
    //inline
    operator () (const typename F::input& p_) const
    {
      point3d p = algebra::to_point<point3d>(p_);

      mln_precondition(fun.domain().has(p));
      //FIXME: What about domain?
      if (is_known(p))
        return value(p);
      value(p)    = fun(p);
      is_known(p) = true;
      return value(p);
    }

    //FIXME: 3d -> //mln_dim(ml_input(input))
    mutable image3d<mln_result(F)> value;
    mutable image3d<bool>          is_known;
    const F&                       fun;
  };
  */

  // Box

  template <typename P>
  inline
  box<P>            //dif
  enlarge(const box<P>& box, unsigned b)
  {
    mln::box<P> nbox(box);

    for (unsigned i = 0; i < P::dim; ++i)
    {
      nbox.pmin()[i] -= b;
      nbox.pmax()[i] += b;
    }
    return nbox;
  }

  template <typename P>
  box<P> bigger(const box<P>& a, const box<P>& b)
  {
    P pmin,pmax;

    for (unsigned i = 0; i < P::dim; i++)
      {
        pmin[i] = (a.pmin()[i] < b.pmin()[i]) ? a.pmin()[i] : b.pmin()[i];
        pmax[i] = (a.pmax()[i] > b.pmax()[i]) ? a.pmax()[i] : b.pmax()[i];
      }

    return box<P>(pmin, pmax);
  }


  //FIXME: move?
  namespace convert
  {

//     // to_p_array
//     template <typename I>
//     inline
//     p_array<mln_psite(I)>
//     to_p_array(const Image<I>& img_)
//     {
//       const I& img = exact(img_);

//       p_array<mln_psite(I)> a;

//       mln_piter(I) p(img.domain());
//       for_all(p)
//         if (img(p))
//           a.append(p);

//       return a;
//     }


    template < typename P >
    inline
    box<point2d>
    to_box2d(const box<P>& b)
    {
      point2d pmin(b.pmin()[0], b.pmin()[1]);
      point2d pmax(b.pmax()[0], b.pmax()[1]);

      return box<point2d>(pmin, pmax);
    }


    //FIXME: to write
    template <typename P>
    inline
    image2d<bool>
    to_image2d(const p_array<P>& a)
    {
      image2d<bool> output (to_box2d(a.bbox()), 0);
      for (size_t i = 0; i < a.nsites(); i++)
        {
          point2d p(a[i][0], a[i][1]);
          if (output.has(p))
            output(p) = true;
        }
      return output;
    }

    // to_pointNd

    //FIXME: Should be call projection
    //point3d -> point2d
    template <typename T>
    inline
    point<grid::square, T>
    to_point2d(const point<grid::cube, T>& p)
    {
      return point<grid::square, T>(p[0], p[1]);
    }

    //point2d -> point2d
    template <typename T>
    inline
    point<grid::square, T>&
    to_point2d(const point<grid::square, T>& p)
    {
      return p;
    }

    //point2d -> point3d
    template <typename T>
    inline
    point<grid::cube, T>
    to_point3d(const point<grid::square, T>& p)
    {
      return point<grid::cube, T>(p[0], p[1], 0);
    }
    //point3d -> point3d
    template <typename T>
    inline
    point<grid::cube, T>&
    to_point3d(const point<grid::cube, T>& p)
    {
      return p;
    }


    // to_imageNd


    //const return a const
    template <typename T>
    inline
    const image3d<T>&
    to_image3d(const image3d<T>& img)
    {
      return img;
    }
    //non const return non const
    template <typename T>
    image3d<T>&
    to_image3d(image3d<T>& img)
    {
      return img;
    }

    template <typename T>
    inline
    image3d<T>
    to_image3d(const image2d<T>& img)
    {
      point3d pmin(img.domain().pmin()[0], img.domain().pmin()[1], 0);
      point3d pmax(img.domain().pmax()[0], img.domain().pmax()[1], 0);
      image3d<T> img3d(box3d(pmin,pmax));

      mln_piter(image3d<T>) p(img3d.domain());
      for_all(p)
        img3d(p) = exact(img)(point2d(p[0],p[1]));

      return img3d;
    }

  } // end of namespace convert


} // end of namespace mln

#endif // REGISTRATION_TOOLS_HH
