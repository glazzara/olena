#ifndef MLN_CORE_CONCEPT_IMAGE_HH
# define MLN_CORE_CONCEPT_IMAGE_HH

/*! \file mln/core/concept/image.hh
 * \brief This file defines the concept of mln::Image.
 */

# include <mln/core/concept/point_set.hh>


namespace mln
{

  /*! \brief Base class for implementation of image classes.
   *
   * \see mln::doc::Image for a complete documentation of this class
   * contents.
   */
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

      template <typename T>
      struct change_value
      {
        typedef ret;
      };


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

    typedef mln_pset(E)  pset;
    typedef mln_point(E) point;
    typedef mln_psite(E) psite;
    
    typedef mln_piter(E)     piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    const box_<point>& (E::*m2)() const = & E::bbox;
    m2 = 0;

    // to be provided in concrete image classes:

    typedef mln_value(E)  value;
    typedef mln_rvalue(E) rvalue;
    typedef mln_lvalue(E) lvalue;

    typedef mln_ch_value(E, value) change;
    
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
