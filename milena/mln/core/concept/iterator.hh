#ifndef MLN_CORE_CONCEPT_ITERATOR_HH
# define MLN_CORE_CONCEPT_ITERATOR_HH

/*! \file mln/core/concept/iterator.hh
 * \brief This file defines the concept of mln::Iterator.
 */

# include <mln/core/concept/object.hh>

/*! \brief Loop to browse all the elements targetted by the iterator
 * \p x.
 */
# define for_all(x) for(x.start(); x.is_valid(); x.next())


namespace mln
{
  /*! \brief Base class for implementation classes that are iterators.
   *
   * \see mln::doc::Iterator for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Iterator : public Object<E>
  {
    /*
      bool is_valid() const;
      void invalidate();
      void start();
      void next_();
    */

    /*! \brief Go to the next element.
     *
     * \warning This is a final method; iterator classes should not
     * re-defined this method.  The actual "next" operation has to be
     * defined through the \em next_ method.
     *
     * \pre The iterator is valid.
     */ 
    void next(); // final

  protected:
    Iterator();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  void Iterator<E>::next() // final
  {
    assert(exact(this)->is_valid());
    exact(this)->next_();
  }

  template <typename E>
  Iterator<E>::Iterator()
  {
    bool (E::*m1)() const = & E::is_valid;
    m1 = 0;
    void (E::*m2)() = & E::invalidate;
    m2 = 0;
    void (E::*m3)() = & E::start;
    m3 = 0;
    void (E::*m4)() = & E::next_;
    m4 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_ITERATOR_HH
