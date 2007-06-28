#ifndef MLN_CORE_CONCEPT_PSITE_HH
# define MLN_CORE_CONCEPT_PSITE_HH

# include <core/concept/genpoint.hh>


namespace mln
{

  template <typename E>
  struct Psite : public Object<E>,
		 public GenPoint<E>
  {
    /*
    const point* pointer() const
    {
      return & (exact(this)->operator point());
    }
    */

  protected:
    Psite();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Psite<E>::Psite()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PSITE_HH
