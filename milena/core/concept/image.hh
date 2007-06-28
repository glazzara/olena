#ifndef MLN_CORE_CONCEPT_IMAGE_HH
# define MLN_CORE_CONCEPT_IMAGE_HH

# include <core/concept/point_set.hh>


namespace mln
{

  template <typename E>
  struct Image : public Object<E>
  {
    /*
      // to be provided in concrete image classes:

      typedef value;
      typedef rvalue;
      typedef lvalue;

      bool has_data() const;

      bool owns_(const psite& p) const; // default is like "has(p)"
      const pset& domain() const;

      rvalue operator()(const psite& p) const;
      lvalue operator()(const psite& p);


      // provided by internal::image_base_:

      typedef pset;
      typedef point;
      typedef psite;

      typedef piter;
      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      const box_<point>& bbox() const;
     */

  protected:
    Image();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Image<E>::Image()
  {
    // provided by internal::image_base_:

    typedef oln_pset(E)  pset;
    typedef oln_point(E) point;
    typedef oln_psite(E) psite;
    
    typedef oln_piter(E)     piter;
    typedef oln_fwd_piter(E) fwd_piter;
    typedef oln_bkd_piter(E) bkd_piter;
    
    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    const box_<point>& (E::*m2)() const = & E::bbox;
    m2 = 0;

    // to be provided in concrete image classes:

    typedef oln_value(E)  value;
    typedef oln_rvalue(E) rvalue;
    typedef oln_lvalue(E) lvalue;
    
    bool (E::*m3)() const = & E::has_data;
    m3 = 0;
    bool (E::*m4)(const psite& p) const = & E::owns_;
    m4 = 0;
    const pset& (E::*m5)() const = & E::domain;
    m5 = 0;

    rvalue (E::*m6)(const psite& p) const = & E::operator();
    m6 = 0;
    lvalue (E::*m7)(const psite& p) = & E::operator();
    m7 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_IMAGE_HH
