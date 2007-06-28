#ifndef MLN_CORE_EXACT_HH
# define MLN_CORE_EXACT_HH

# include <core/concept/object.hh>


namespace mln
{

  // exact

  template <typename E>
  E* exact(Object<E>* ptr);

  template <typename E>
  const E* exact(const Object<E>* ptr);

  template <typename E>
  E& exact(Object<E>& ref);

  template <typename E>
  const E& exact(const Object<E>& ref);


  // force_exact

  template <typename E, typename O>
  E* force_exact(O* ptr);

  template <typename E, typename O>
  const E* force_exact(const O* ptr);

  template <typename E, typename O>
  E& force_exact(O& ref);

  template <typename E, typename O>
  const E& force_exact(const O& ref);



# ifndef MLN_INCLUDE_ONLY

  // exact

  template <typename E>
  E* exact(Object<E>* ptr)
  {
    return (E*)(void*)ptr;
  }

  template <typename E>
  const E* exact(const Object<E>* ptr)
  {
    return (const E*)(const void*)ptr;
  }

  template <typename E>
  E& exact(Object<E>& ref)
  {
    return *(E*)(void*)(&ref);
  }

  template <typename E>
  const E& exact(const Object<E>& ref)
  {
    return *(const E*)(const void*)(&ref);
  }

  // force_exact

  template <typename E, typename O>
  E* force_exact(O* ptr)
  {
    return (E*)(void*)ptr;
  }

  template <typename E, typename O>
  const E* force_exact(const O* ptr)
  {
    return (const E*)(const void*)ptr;
  }

  template <typename E, typename O>
  E& force_exact(O& ref)
  {
    return *(E*)(void*)(&ref);
  }

  template <typename E, typename O>
  const E& force_exact(const O& ref)
  {
    return *(const E*)(const void*)(&ref);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_EXACT_HH
