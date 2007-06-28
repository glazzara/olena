#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH


# define oln_point(T)      typename T::point
# define oln_dpoint(T)     typename T::dpoint
# define oln_psite(T)      typename T::psite
# define oln_pset(T)       typename T::pset
# define oln_box(T)        typename T::box
# define oln_coord(T)      typename T::coord

# define oln_piter(T)      typename T::piter
# define oln_fwd_piter(T)  typename T::fwd_piter
# define oln_bkd_piter(T)  typename T::bkd_piter

# define oln_qiter(T)      typename T::qiter
# define oln_fwd_qiter(T)  typename T::fwd_qiter
# define oln_bkd_qiter(T)  typename T::bkd_qiter

# define oln_niter(T)      typename T::niter
# define oln_fwd_niter(T)  typename T::fwd_niter
# define oln_bkd_niter(T)  typename T::bkd_niter

# define oln_value(T)      typename T::value
# define oln_rvalue(T)     typename T::rvalue
# define oln_lvalue(T)     typename T::lvalue

#endif // ! MLN_CORE_MACROS_HH
