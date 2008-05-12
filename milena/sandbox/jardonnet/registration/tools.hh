#ifndef REGISTRATION_TOOL_HH
# define REGISTRATION_TOOL_HH

// temporary

# include <mln/algebra/mat.hh>
# include <mln/core/p_array.hh>

# include "quat7.hh"


namespace mln
{

  namespace registration
  {
    
    template <typename P, typename M>
    void mean_stddev(const p_array<P>& c,
		     const M&          map,
		     float& mean,
		     float& stddev)
    {
      //mean + length
      std::vector<float> length(c.npoints());
      mean = 0;

      for (size_t i = 0; i < c.npoints(); i++)
	{
	  float f = norm::l2(algebra::vec<3,int> (c[i] - map(c[i])));
	  length[i] = f;
	  mean += f;
	}
      mean /= c.npoints();
  
      //standar variation
      stddev = 0;
      for (size_t i = 0; i < c.npoints(); i++)
	stddev += (length[i] - mean) * (length[i] - mean);
      stddev /= c.npoints();
      stddev = math::sqrt(stddev);
    }


    //final qk : only use point less than nstddev (2*stddev);
    template <typename P, typename M>
    quat7<P::dim>
    final_qk(const p_array<P>& c,
	     const M&    map,
	     float       nstddev)
    {
      p_array<P> newc;
      algebra::vec<3,float> mu_newc(literal::zero);

      for (size_t i = 0; i < c.npoints(); ++i)
	{
	  algebra::vec<3,float> xki = map(c[i]);
	  algebra::vec<3,float> ci = c[i];

	  if (norm::l2(ci - xki) > nstddev)
	    {
	      newc.append(c[i]);
	      mu_newc += ci;
	    }
	}
      mu_newc /= newc.npoints();

      quat7<P::dim> qk = match(newc, mu_newc, newc, map, newc.npoints());

      return qk;
    }

    //final > nstddev for translation; < nstddev for rotation
    template <typename P, typename M>
    quat7<P::dim>
    final_qk2(const p_array<P>& c,
	      const M&    map,
	      float       nstddev)
    {
      //mu_Xk = center map(Ck)
      algebra::vec<3,float> mu_Xk(literal::zero);
      algebra::vec<3,float> mu_C(literal::zero);
      
      float nb_point = 0;
      for (size_t i = 0; i < c.npoints(); ++i)
	{
	  algebra::vec<3,float> xki = map(c[i]);
	  algebra::vec<3,float> ci = c[i];
	  
	  if (norm::l2(ci - xki) > nstddev)
	    {
	      mu_C += ci;
	      mu_Xk += xki;
	      nb_point++;
	    }
	}
      mu_C  /= nb_point;
      mu_Xk /= nb_point;

      // qT
      const algebra::vec<3,float> qT = mu_C - mu_Xk;

      // qR
      quat7<P::dim> qk = final_qk(c, map, nstddev);
      qk._qT = qT;

      return qk;
    }
    
    
  } // end of namespace mln::registration

  
  //FIXME: groe length
  template <typename P>
  struct closest_point
  {
    typedef P input;
    typedef P result;
    
    closest_point(const p_array<P>& X, const box_<P>& box)
      : X(X), box(box)     
#ifndef NDEBUG
      , i(0)
#endif
    { }
    
    result
    //inline
    operator () (const P& Ck) const
    {
      
#ifndef NDEBUG
      ++i;
#endif
      
      algebra::vec<P::dim,float> Cki = Ck;
      algebra::vec<P::dim,float> best_x = X[0];
      float best_d = norm::l2(Cki - best_x);
      for (size_t j = 1; j < X.npoints(); ++j)
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

    const box_<P>& domain() const
    {
      return box;
    }
    
    const p_array<P>& X;
    const box_<P>     box;

#ifndef NDEBUG
    mutable unsigned i;
#endif 
  };
  

  // FIXME: Should be a morpher ?
  // we could acces domain of a lazy map, iterator etc...
  template < typename F>
  struct lazy_image
  { 
    // Fun is potentially an image.
    lazy_image(F& fun)
      : value(fun.domain()), is_known(fun.domain()), fun(fun)
    {
    }

    // FIXME: hack, remove this constructor
    lazy_image(F& fun, int nrows, int ncols, int nslis)
      : value(nrows, ncols,1), is_known(nrows,ncols,1), fun(fun)
    { }

    // FIXME: gore length
    const mln_result(F)
    //inline
    operator () (const typename F::input& p) const
    {
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
    F&                             fun;
  };

  
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

    // to_p_array
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

    template < typename P >
    inline
    box_<point2d>
    to_box2d(const box_<P>& b)
    {
      point2d pmin(b.pmin()[0], b.pmin()[1]);
      point2d pmax(b.pmax()[0], b.pmax()[1]);

      return box_<point2d>(pmin, pmax);
    }

    
    //FIXME: to write
    template <typename P>
    inline
    image2d<bool>
    to_image2d(const p_array<P>& a)
    {
      image2d<bool> output (to_box2d(a.bbox()), 0);
      for (size_t i = 0; i < a.npoints(); i++)
        {
          point2d p(a[i][0], a[i][1]);
          if (output.has(p))
            output(p) = true;
        }
      return output;
    }

    // to_pointNd

    //FIXME: Should we really provide this
    //point3d -> point2d
    template <typename T>
    inline
    point_<grid::square, T>
    to_point2d(const point_<grid::cube, T>& p)
    {
      return point_<grid::square, T>(p[0], p[1]);
    }
    //point2d -> point2d
    template <typename T>
    inline
    point_<grid::square, T>&
    to_point2d(const point_<grid::square, T>& p)
    {
      return p;
    }

    //point2d -> point3d
    template <typename T>
    inline
    point_<grid::cube, T>
    to_point3d(const point_<grid::square, T>& p)
    {
      return point_<grid::cube, T>(p[0], p[1], 0);
    }
    //point3d -> point3d
    template <typename T>
    inline
    point_<grid::cube, T>&
    to_point3d(const point_<grid::cube, T>& p)
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
      {
        if (p[2] == 0)
          img3d(p) = exact(img)(point2d(p[0],p[1]));
      }
      return img3d;
    }
    
  } // end of namespace convert  
  
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
