#ifndef MLN_CORE_POINT_HH
# define MLN_CORE_POINT_HH

# include <mln/core/concept/point.hh>
# include <mln/core/internal/coord_impl.hh>


namespace mln
{

  // fwd decl
  template <unsigned n, typename C> struct dpoint_;


  template <unsigned n, typename C>
  struct point_ : public Point< point_<n,C> >,
		  public internal::mutable_coord_impl_< n, C, point_<n,C> >
  {
    enum { dim = n };
    typedef C coord;
    typedef dpoint_<n,C> dpoint;

    C  operator[](unsigned i) const;
    C& operator[](unsigned i);

    point_();
    point_(C c);
    void set_all(C c);

  protected:
    C coord_[n];
  };


# ifndef MLN_INCLUDE_ONLY

  template <unsigned n, typename C>
  C point_<n,C>::operator[](unsigned i) const
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  C& point_<n,C>::operator[](unsigned i)
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  point_<n,C>::point_()
  {
  }

  template <unsigned n, typename C>
  point_<n,C>::point_(C c)
  {
    set_all(c);
  }

  template <unsigned n, typename C>
  void point_<n,C>::set_all(C c)
  {
    for (unsigned i = 0; i < n; ++i)
      coord_[i] = c;
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_POINT_HH
