#ifndef MLN_CORE_CONCEPT_PITER_HH
# define MLN_CORE_CONCEPT_PITER_HH

# include <core/concept/iterator.hh>
# include <core/concept/genpoint.hh>


namespace mln
{

  template <typename E>
  struct Piter : public Iterator<E>,
		 public GenPoint<E>
  {
    /*
      typedef psite;
      typedef point;

      operator psite() const;
      operator point() const;
      const point* pointer() const;
     */

  protected:
    Piter();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Piter<E>::Piter()
  {
    typedef oln_psite(E) psite;
    typedef oln_point(E) point;
    const point* (E::*m)() const = & E::pointer;
    m = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PITER_HH
