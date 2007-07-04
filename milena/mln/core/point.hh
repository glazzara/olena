#ifndef MLN_CORE_POINT_HH
# define MLN_CORE_POINT_HH

/*! \file mln/core/point.hh
 *
 * \brief This file defines a generic point class.
 */

# include <mln/core/concept/point.hh>
# include <mln/core/internal/coord_impl.hh>


namespace mln
{

  // fwd decl
  template <unsigned n, typename C> struct dpoint_;


  /*! \brief Generic point class.
   *
   * Parameters are \c n the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <unsigned n, typename C>
  struct point_ : public Point< point_<n,C> >,
		  public internal::mutable_coord_impl_< n, C, point_<n,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0 
     */
    enum { dim = n };

    /*! \brief Dpoint associated type.
     */
    typedef dpoint_<n,C> dpoint;

    /*! Coordinate associated type.
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
    point_();

    /*! \brief Constructor; all coordinates are set to the value \p c.
     */
    point_(C c);

    /*! \brief Set all coordinates to the value \p c.
     */
    void set_all(C c);

  protected:
    C coord_[n];
  };


# ifndef MLN_INCLUDE_ONLY

  template <unsigned n, typename C>
  C point_<n,C>::operator[](unsigned i) const
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  C& point_<n,C>::operator[](unsigned i)
  {
    assert(i < n);
    return coord_[i];
  }

  template <unsigned n, typename C>
  point_<n,C>::point_()
  {
  }

  template <unsigned n, typename C>
  point_<n,C>::point_(C c)
  {
    set_all(c);
  }

  template <unsigned n, typename C>
  void point_<n,C>::set_all(C c)
  {
    for (unsigned i = 0; i < n; ++i)
      coord_[i] = c;
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_POINT_HH
