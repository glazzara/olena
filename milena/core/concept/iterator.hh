#ifndef MLN_CORE_CONCEPT_ITERATOR_HH
# define MLN_CORE_CONCEPT_ITERATOR_HH

# include <core/concept/object.hh>


# define for_all(x) for(x.start(); x.is_valid(); x.next())


namespace mln
{

  template <typename E>
  struct Iterator : public Object<E>
  {
    /*
      bool is_valid() const;
      void invalidate();
      void start();
      void next_();
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
