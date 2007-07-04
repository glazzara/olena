#ifndef MLN_CORE_CONCEPT_OBJECT_HH
# define MLN_CORE_CONCEPT_OBJECT_HH

/*! \file mln/core/concept/object.hh
 * \brief This file contains the top milena class.
 */

# include <cassert>
# include <iostream>

# include <mln/core/macros.hh>


/*! \namespace mln
 * \brief The namespace mln corresponds to the Milena (mini-Olena) project.
 *
 * The contents of mln mimics the contents of the olena project but
 * in a simplified way.  Some classes have the same name in both
 * projects and roughly have the same behavior.
 *
 * \warning The Milena project is independent from the Olena
 * project; the user has to choose between both the project she
 * wants to work with.
 */
namespace mln
{

  /*! \brief Base class for almost every class defined in milena.
   *
   * The parameter \a E is the exact type.
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


# include <mln/core/exact.hh>
# include <mln/core/ops.hh>


#endif // ! MLN_CORE_CONCEPT_OBJECT_HH
