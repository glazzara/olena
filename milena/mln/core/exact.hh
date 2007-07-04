#ifndef MLN_CORE_EXACT_HH
# define MLN_CORE_EXACT_HH

/*! \file mln/core/exact.hh
 * \brief This file defines the mln::exact downcast routines.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  /*! \brief Exact cast routine for mln objects.
   * 
   * This set of routines can be used to downcast an object towards
   * its exact type.  The only argument, respectively \p ptr or \p
   * ref, should be an mln::Object.
   *
   * \c The parameter E is the exact type of the object.
   *
   * \return The return follows the nature of the argument (either a
   * pointer or a reference, const or not).
   */ 
  /// \{

  template <typename E>
  E* exact(Object<E>* ptr);

  template <typename E>
  const E* exact(const Object<E>* ptr);

  template <typename E>
  E& exact(Object<E>& ref);

  template <typename E>
  const E& exact(const Object<E>& ref);

  /// \}



  /*! \brief Violent exact cast routine.
   *
   * This cast is an alternative to the mln::exact cast.  It is used
   * for objects than do not derive from mln::Object.
   * 
   * It preserves the nature (pointer or reference, const or mutable)
   * of its argument.  The parameter \a E should not have qualifiers.
   *
   * \warning Prefer not to use this cast!   
   *
   * \see mln::exact
   * \todo Move into mln::internal.
   */ 
  /// \{

  template <typename E, typename O>
  E* force_exact(O* ptr);

  template <typename E, typename O>
  const E* force_exact(const O* ptr);

  template <typename E, typename O>
  E& force_exact(O& ref);

  template <typename E, typename O>
  const E& force_exact(const O& ref);

  /// \}



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
