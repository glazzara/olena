#ifndef MLN_CORE_CONCEPT_BOX_HH
# define MLN_CORE_CONCEPT_BOX_HH

# include <core/concept/point_set.hh>


namespace mln
{

  template <typename E>
  struct Box : public Point_Set<E>
  {
    /*
      const point& pmin() const;
      const point& pmax() const;
    */

    const E& bbox() const; // final

  protected:
    Box();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  const E& Box<E>::bbox() const
  {
    return exact(*this);
  }

  template <typename E>
  Box<E>::Box()
  {
    typedef mln_point(E) point;
    point (E::*m1)() const = & E::pmin;
    m1 = 0;
    point (E::*m2)() const = & E::pmax;
    m2 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_BOX_HH
