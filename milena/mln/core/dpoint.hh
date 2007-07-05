#ifndef MLN_CORE_DPOINT_HH
# define MLN_CORE_DPOINT_HH

/*! \file mln/core/dpoint.hh
 *
 * \brief Definition of the generic delta-point class mln::dpoint_.
 */

# include <mln/core/concept/dpoint.hh>
# include <mln/core/internal/coord_impl.hh>


namespace mln
{

  // fwd decl
  template <unsigned n, typename C> struct point_;


  /*! \brief Generic delta-point class.
   *
   * Parameters are \c n the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <unsigned n, typename C>
  struct dpoint_ : public Dpoint< dpoint_<n,C> >,
		   public internal::mutable_coord_impl_< n, C, dpoint_<n,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0 
     */
    enum { dim = n };

    /*! \brief Point associated type.
     */
    typedef point_<n,C> point;

    /*! \brief Coordinate associated type.
     */
    typedef C coord;

    /*! \brief Read-only access to the \p i-th coordinate value.
     * \param[in] i The coordinate index.
     * \pre \p i < \c dim
     */
    C  operator[](unsigned i) const;

    /*! \brief Read-write access to the \p i-th coordinate value.
     * \param[in] i The coordinate index.
     * \pre \p i < \c dim
     */
    C& operator[](unsigned i);

    /*! \brief Constructor without argument.
     */
    dpoint_();

    /*! \brief Constructor; all coordinates are set to the value \p c.
     */
    dpoint_(C c);

    /*! \brief Set all coordinates to the value \p c.
     */
    void set_all(C c);

  protected:
    C coord_[n];
  };


# ifndef MLN_INCLUDE_ONLY

  template <unsigned n, typename C>
  C dpoint_<n,C>::operator[](unsigned i) const
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  C& dpoint_<n,C>::operator[](unsigned i)
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  dpoint_<n,C>::dpoint_()
  {
  }

  template <unsigned n, typename C>
  dpoint_<n,C>::dpoint_(C c)
  {
    set_all(c);
  }

  template <unsigned n, typename C>
  void dpoint_<n,C>::set_all(C c)
  {
    for (unsigned i = 0; i < n; ++i)
      coord_[i] = c;
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_DPOINT_HH
