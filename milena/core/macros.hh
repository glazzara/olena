#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH


# define mln_point(T)      typename T::point
# define mln_dpoint(T)     typename T::dpoint
# define mln_psite(T)      typename T::psite
# define mln_pset(T)       typename T::pset
# define mln_box(T)        typename T::box
# define mln_coord(T)      typename T::coord

# define mln_piter(T)      typename T::piter
# define mln_fwd_piter(T)  typename T::fwd_piter
# define mln_bkd_piter(T)  typename T::bkd_piter

# define mln_qiter(T)      typename T::qiter
# define mln_fwd_qiter(T)  typename T::fwd_qiter
# define mln_bkd_qiter(T)  typename T::bkd_qiter

# define mln_niter(T)      typename T::niter
# define mln_fwd_niter(T)  typename T::fwd_niter
# define mln_bkd_niter(T)  typename T::bkd_niter

# define mln_value(T)      typename T::value
# define mln_rvalue(T)     typename T::rvalue
# define mln_lvalue(T)     typename T::lvalue

#endif // ! MLN_CORE_MACROS_HH
