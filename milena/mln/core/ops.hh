#ifndef MLN_CORE_OPS_HH
# define MLN_CORE_OPS_HH

/*! \file mln/core/ops.hh
 * \brief Definitions of some operators.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/exact.hh>


namespace mln
{

  /*! \brief General definition of the "not equal to" operator.
   *
   * The "not equal to" operator is here defined for every milena
   * objects.  It relies on the definition of the "equal to" operator.
   * It returns "not (lhs == rhs)".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than" operator.
   *
   * The "greater than" operator is here defined for every milena
   * objects.  It relies on the definition of the "less than"
   * operator.  It returns "rhs < lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than or equal to"
   *  operator.
   *
   * The "greater than or equal to" operator is here defined for
   * every milena objects.  It relies on the definition of the "less
   * than or equal to" operator.  It returns "rhs <= lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief Default definition of the "less than or equal to"
   *  operator.
   *
   * A default version of the "less than or equal to" operator is
   * defined for every milena objects.  It relies on the definition of
   * the "less than" operator.  It returns "not (rhs < lhs)".
   *
   * \warning In the case of partial ordering between objects, this
   * operator has to be re-defined.
   */
  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return not (exact(lhs) == exact(rhs));
  }

  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) < exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) <= exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    // if partial ordering, this operator should be re-defined!
    return not (exact(rhs) < exact(lhs));
  }  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_OPS_HH
