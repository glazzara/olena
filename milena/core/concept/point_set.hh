#ifndef MLN_CORE_CONCEPT_POINT_SET_HH
# define MLN_CORE_CONCEPT_POINT_SET_HH

# include <core/concept/point.hh>
# include <core/concept/piter.hh>


namespace mln
{

  template <typename E>
  struct Point_Set : public Object<E>
  {
    /*
      typedef point;
      typedef psite;
      typedef piter;
      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      const box_<point>& bbox() const;
     */

  protected:
    Point_Set();
  };


# ifndef MLN_INCLUDE_ONLY

  // fwd decl
  template <typename P> struct box_;

  template <typename E>
  Point_Set<E>::Point_Set()
  {
    typedef mln_point(E) point;
    typedef mln_psite(E) psite;
    typedef mln_piter(E) piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;
    
    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    const box_<point>& (E::*m2)() const = & E::bbox;
    m2 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_POINT_SET_HH
