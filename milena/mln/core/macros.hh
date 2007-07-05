#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH

/*! \file mln/core/macros.hh
 * Definition of the set of milena macros.
 */

/// \brief Shortcut to access the point type associated to T.
# define mln_point(T)      typename T::point

/// \brief Shortcut to access the dpoint type associated to T.
# define mln_dpoint(T)     typename T::dpoint

/// \brief Shortcut to access the psite type associated to T.
# define mln_psite(T)      typename T::psite

/// \brief Shortcut to access the pset type associated to T.
# define mln_pset(T)       typename T::pset

/// \brief Shortcut to access the box type associated to T.
# define mln_box(T)        typename T::box

/// \brief Shortcut to access the coord type associated to T.
# define mln_coord(T)      typename T::coord


/// \brief Shortcut to access the piter type associated to T.
# define mln_piter(T)      typename T::piter

/// \brief Shortcut to access the fwd_piter type associated to T.
# define mln_fwd_piter(T)  typename T::fwd_piter

/// \brief Shortcut to access the bkd_piter type associated to T.
# define mln_bkd_piter(T)  typename T::bkd_piter


/// \brief Shortcut to access the qiter type associated to T.
# define mln_qiter(T)      typename T::qiter

/// \brief Shortcut to access the fwd_qiter type associated to T.
# define mln_fwd_qiter(T)  typename T::fwd_qiter

/// \brief Shortcut to access the bkd_qiter type associated to T.
# define mln_bkd_qiter(T)  typename T::bkd_qiter


/// \brief Shortcut to access the niter type associated to T.
# define mln_niter(T)      typename T::niter

/// \brief Shortcut to access the fwd_niter type associated to T.
# define mln_fwd_niter(T)  typename T::fwd_niter

/// \brief Shortcut to access the bkd_niter type associated to T.
# define mln_bkd_niter(T)  typename T::bkd_niter


/// \brief Shortcut to access the value type associated to T.
# define mln_value(T)      typename T::value

/// \brief Shortcut to access the rvalue type associated to T.
# define mln_rvalue(T)     typename T::rvalue

/// \brief Shortcut to access the lvalue type associated to T.
# define mln_lvalue(T)     typename T::lvalue



/*! \brief Shortcut to change the value type of an image type.
 *
 * \c I is an image type
 * \c T is the new value type
 */
# define mln_ch_value(I, T) typename I::template change_value<T>::ret


#endif // ! MLN_CORE_MACROS_HH
