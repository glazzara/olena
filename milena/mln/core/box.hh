#ifndef MLN_CORE_BOX_HH
# define MLN_CORE_BOX_HH

/*! \file mln/core/box.hh
 *
 * \brief This file defines a generic box class.
 */

# include <mln/core/concept/box.hh>
# include <mln/core/point.hh>


namespace mln
{

  // fwd decl
  template <typename P> struct box_fwd_piter_;
  template <typename P> struct box_bkd_piter_;


  /*! \brief Generic box class.
   *
   * Parameter \c P is the corresponding type of point.
   */
  template <typename P>
  struct box_ : public Box< box_<P> >
  {
    /*! \brief Psite associated type.
     */
    typedef P psite;

    /*! \brief Point associated type.
     */
    typedef P point;
    
    /*! \brief Forward Piter associated type.
     */
    typedef box_fwd_piter_<P> fwd_piter;

    /*! \brief Backward Piter associated type.
     */
    typedef box_bkd_piter_<P> bkd_piter;

    /*! \brief Piter associated type.
     */
    typedef fwd_piter piter;

    /*! \brief Minimum point.
     */
    P  pmin() const;

    /*! \brief Reference to the minimum point.
     */
    P& pmin();

    /*! \brief Maximum point.
     */
    P  pmax() const;

    /*! \brief Reference to the maximum point.
     */
    P& pmax();

    /*! \brief Constructor without argument.
     */
    box_();

    /*! \brief Constructor of a box going from \p pmin to \p pmax.
     */
    box_(const point& pmin, const point& pmax);

    /*! \brief Test if \p p belongs to the box. 
     *
     * \param[in] p A point site.
     */
    bool has(const P& p) const;

  protected:
    P pmin_, pmax_;
  };


  /*! \brief Print a generic box \p b into the output stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] b A generic box.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::box_
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b);


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  P
  box_<P>::pmin() const
  {
    return pmin_;
  }

  template <typename P>
  P&
  box_<P>::pmin()
  {
    return pmin_;
  }

  template <typename P>
  P
  box_<P>::pmax() const
  {
    return pmax_;
  }

  template <typename P>
  P&
  box_<P>::pmax()
  {
    return pmax_;
  }

  template <typename P>
  box_<P>::box_()
  {
  }
  
  template <typename P>
  box_<P>::box_(const point& pmin, const point& pmax)
    : pmin_(pmin),
      pmax_(pmax)
  {
  }

  template <typename P>
  bool
  box_<P>::has(const P& p) const
  {
    for (unsigned i = 0; i < P::dim; ++i)
      if (p[i] < pmin_[i] or p[i] > pmax_[i])
	return false;
    return true;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b)
  {
    return ostr << "[" << b.pmin() << ".." << b.pmax() << ']';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/box_piter.hh>


#endif // ! MLN_CORE_BOX_HH
