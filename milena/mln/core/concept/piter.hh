#ifndef MLN_CORE_CONCEPT_PITER_HH
# define MLN_CORE_CONCEPT_PITER_HH

/*! \file mln/core/concept/piter.hh
 *
 * \brief Definition of the concept of mln::Piter.
 */

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/genpoint.hh>


namespace mln
{

  /*! \brief Base class for implementation of classes of iterator on
   *  points.
   *
   * An iterator on points is an iterator that browse over a set of
   * points.
   *
   * \see mln::doc::Piter for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Piter : public Iterator<E>,
		 public GenPoint<E>
  {
    /*
      typedef psite;
      operator psite() const;
     */

  protected:
    Piter();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Piter<E>::Piter()
  {
    typedef mln_psite(E) psite;
    psite (E::*m)() const = & E::operator psite;
    m = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PITER_HH
