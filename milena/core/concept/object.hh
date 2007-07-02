#ifndef MLN_CORE_CONCEPT_OBJECT_HH
# define MLN_CORE_CONCEPT_OBJECT_HH

/*! \file mln/core/concept/object.hh
 * This file contains the top milena class.
 */

# include <cassert>
# include <iostream>

# include <core/macros.hh>


namespace mln
{

  /*! \brief Base class for almost every class defined in milena.
   *
   *  The parameter \a E is the exact type.
   */
  template <typename E>
  struct Object
  {
  protected:
    Object();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Object<E>::Object()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <core/exact.hh>
# include <core/ops.hh>


#endif // ! MLN_CORE_CONCEPT_OBJECT_HH
