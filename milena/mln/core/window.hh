#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

/*! \file mln/core/window.hh
 *
 * \brief Definition of the generic window class mln::window_.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  /*! \brief Generic window class.
   *
   * This type of window is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D>
  struct window_ : public Window< window_<D> >,
		   public internal::set_of_<D>
  {
    /*! \brief Piter type to browse the points of a generic window
     * w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_qiter;

    /*! \brief Piter type to browse the points of a generic window
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_qiter;

    /*! \brief Same as fwd_qiter.
     */
    typedef fwd_qiter qiter;

    /*! \brief Constructor without argument.
     *
     * The constructed window is empty. 
     */
    window_();

    /*! \brief Test if the window is centered.
     *
     * \return True if the delta-point 0 belongs to the window.
     */
    bool is_centered() const;

    /*! \brief Test if the window is symmetric.
     *
     * \todo Implementation!
     */
    bool is_symmetric() const;
  };
 


# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  window_<D>::window_()
  {
  }

  template <typename D>
  bool window_<D>::is_centered() const
  {
    static const D origin(0);
    return this->has(origin);
  }

  template <typename D>
  bool window_<D>::is_symmetric() const
  {
    assert(0); // FIXME: nyi!
    return false;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpoints_piter.hh>


#endif // ! MLN_CORE_WINDOW_HH
