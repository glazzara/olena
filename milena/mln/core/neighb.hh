#ifndef MLN_CORE_NEIGHB_HH
# define MLN_CORE_NEIGHB_HH

/*! \file mln/core/neighb.hh
 *
 * \brief Definition of the generic neighborhood class mln::neighb_.
 */

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  /*! \brief Generic neighborhood class.
   *
   * This neighborhood of window is just like a set of delta-points.
   * The parameter is \c D, type of delta-point.
   */
  template <typename D>
  struct neighb_ : public Neighborhood< neighb_<D> >,
		   public internal::set_of_<D>
  {
    /*! \brief Piter type to browse the points of a generic
     * neighborhood w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_niter;

    /*! \brief Piter type to browse the points of a generic
     * neighborhood w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_niter;

    /*! \brief Same as fwd_niter.
     */
    typedef fwd_niter niter;

    /*! \brief Constructor without argument.
     *
     * The constructed neighborhood is empty. You have to use insert()
     * to proceed to the neighborhood definition.
     */
    neighb_();

    /*! \brief Insert a delta-point \p dp in the neighborhood
     *  definition.
     *
     * \param[in] dp The delta-point to insert.
     *
     * This method also insert the symmetrical delta-point, - \p dp,
     * in the neighborhood definition; thus the client has not to
     * ensure the symmetry property; that is automatic.
     */
    void insert(const D& dp);
  };
 

# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  neighb_<D>::neighb_()
  {
  }

  template <typename D>
  void
  neighb_<D>::insert(const D& dp)
  {
    typedef internal::set_of_<D> super;
    this->super::insert( dp);
    this->super::insert(-dp);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpoints_piter.hh>


#endif // ! MLN_CORE_NEIGHB_HH
