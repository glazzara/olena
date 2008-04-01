#ifndef REGISTRATION_TOOL_HH
# define REGISTRATION_TOOL_HH

// temporary

# include <mln/algebra/mat.hh>
# include <mln/core/p_array.hh>


namespace mln
{

  
  //FIXME: Shall we use something *really* lazy
  template <typename I>
  struct lazy_map
  {
    template <typename P>
    lazy_map(const box_<P>& domain)
      : map(domain), known(domain)
    { }

    lazy_map(int nrows, int ncols, int bdr = 3)
      : map(nrows, ncols, bdr), known(nrows,ncols,bdr)
    { }

    mln_ch_value(I, mln_point(I)) map;
    mln_ch_value(I, bool)         known;
  };


  // Point
  
  template <typename P>
  P min(const P& a, const P& b)
  {
    if (a > b)
      return b;
    return a;
  }

  template <typename P>
  P max(const P& a, const P& b)
  {
    if (a < b)
      return b;
    return a;
  }

  
  // Box

  template <typename P>
  inline
  const box_<P>&            //dif
  enlarge(const box_<P>& box, unsigned b)
  {
    for (unsigned i = 0; i < P::dim; ++i)
    {
      box.pmin()[i] -= b;
      box.pmax()[i] += b;
    }
    return box;
  }
  
  template <typename P>
  box_<P> bigger(box_<P> a, box_<P> b)
  {
    P pmin,pmax;
    
    pmin = min(a.pmin(), b.pmin());
    pmax = max(a.pmax(), b.pmax());
    
    return box_<P>(pmin, pmax);
  }


  //FIXME: move?
  namespace convert
  {

    template <typename I>
    inline
    p_array<mln_point(I)>
    to_p_array(const Image<I>& img_)
    {
      const I& img = exact(img_);
      
      p_array<mln_point(I)> a;
        
      mln_piter(I) p(img.domain());
      for_all(p)
        if (img(p))
          a.append(p);
      return a;
    }

    //FIXME: to write
    /*
    template <typename A>
    inline
    image2d<mln_value(A)>
    to_image2d(const A& a)
    {
      image2d<mln_value(A)> img(a.bbox());
      for (size_t i = 0; i < a.npoints(); i++)
        {
          point2d p(res[i][0], res[i][1]);
          //FIXME: BOF
          //if (output.has(p))
          output(p) = true;
        }
    }
    */
    
    template <typename T>
    inline
    const image3d<T>&
    to_image_3d(const image3d<T>& img)
    {
      return img;
    }
      
    template <typename T>
    image3d<T>&
    to_image_3d(image3d<T>& img)
    {
      return img;
    }
    
    template <typename T>
    inline
    image3d<T>
    to_image_3d(const image2d<T>& img)
    {
      point3d pmin(img.domain().pmin()[0], img.domain().pmin()[1], 0);
      point3d pmax(img.domain().pmax()[0], img.domain().pmax()[1], 0);
      image3d<T> img3d(box3d(pmin,pmax));
        
      mln_piter(image3d<T>) p(img3d.domain());
      for_all(p)
      {
        if (p[2] == 0)
          img3d(p) = exact(img)(point2d(p[0],p[1]));
      }
      return img3d;
    }
    
  } // end of namespace convert


  
  namespace fun
  {
    //FIXME: temporary
    template <typename C, typename T= float>
    struct cham : public Function_p2v< cham<C,T> >
    {
      typedef T result;
      //bad
      T operator()(dpoints_fwd_piter<mln::dpoint_<mln::grid::cube, int> >& v) const
      {
        C o = C::origin;
        if (v < o)
          return 1.;
        else
          return 0.;
      }
    };
  } // end of namespace fun


  
  namespace algebra
  {
      
    template<unsigned n, unsigned m, typename T>
    mat<m,n,T>
    trans(const mat<n,m,T>& matrice)
    {
      mat<m,n,T> tmp;
      for (unsigned i = 0; i < n; ++i)
        for (unsigned j = 0; j < m; ++j)
          tmp(j,i) = matrice(i,j);
      return tmp;
    }
    
    // trace
    
    template<unsigned n, typename T> inline
    float tr(const mat<n,n,T>& m)
    {
      float f = 0.f;
      for (unsigned i = 0; i < n; ++i)
        f += m(i,i);
      return f;
    }
      
  } // end of namespace algebra
    
} // end of namespace mln

#endif // REGISTRATION_TOOLS_HH
