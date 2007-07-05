#ifndef MLN_CORE_RECTANGLE2D_HH
# define MLN_CORE_RECTANGLE2D_HH

/*! \file mln/core/rectangle2d.hh
 *
 * \brief Definition of the mln::rectangle2d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{
 
  /*! \brief Rectangular window defined on the 2D square grid.
   *
   * A rectangle2d is a 2D window with rectangular shape.  It is
   * centered and symmetrical.
   *
   * For instance:
   *  o o o o o
   *  o o x o o
   *  o o o o o
   * is defined with half_height = 1 and half_width = 2.
   */
  struct rectangle2d : public Window< rectangle2d >,
		       public internal::set_of_<dpoint2d>
  {
    /*! \brief Piter type to browse a rectangle such as: "for each row
     * (increasing), for each column (increasing)."
     */
    typedef dpoints_fwd_piter<dpoint2d> fwd_qiter;

    /*! \brief Piter type to browse a rectangle such as: "for each row
     * (decreasing), for each column (decreasing)."
     */
    typedef dpoints_bkd_piter<dpoint2d> bkd_qiter;

    /*! \brief Same as fwd_qiter.
     */
    typedef fwd_qiter qiter;

    /*! \brief Constructor.
     *
     * \param[in] half_height sic
     * \param[in] half_width sic
     *
     * \pre half_height != 0 and half_width != 0
     */
    rectangle2d(unsigned half_height, unsigned half_width);

    /*! \brief Test if the window is centered.
     *
     * \return True.
     */
    bool is_centered() const;

    /*! \brief Test if the window is symmetric.
     *
     * \return true.
     */
    bool is_symmetric() const;

    /*! \brief Give the rectangle height.
     */
    unsigned height() const;

    /*! \brief Give the rectangle width.
     */
    unsigned width() const;
    
  protected:
    unsigned half_height_, half_width_;
  };


  /*! \brief Print a rectangle window \p win into the output stream \p
   *  ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] win A rectangle window.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::rectangle2d
   */
  std::ostream& operator<<(std::ostream& ostr, const rectangle2d& win);

 

# ifndef MLN_INCLUDE_ONLY

  rectangle2d::rectangle2d(unsigned half_height, unsigned half_width)
    : half_height_(half_height),
      half_width_(half_width)
  {
    assert(half_height != 0 and half_width != 0);
    const int drow = half_height, dcol = half_width;
    for (int row = - drow; row <= drow; ++row)
      for (int col = - dcol; col <= dcol; ++col)
	insert(mk_dpoint2d(row, col));
  }

  bool rectangle2d::is_centered() const
  {
    return true;
  }

  bool rectangle2d::is_symmetric() const
  {
    return true;
  }

  unsigned rectangle2d::height() const
  {
    return 2 * half_height_ + 1;
  }

  unsigned rectangle2d::width() const
  {
    return 2 * half_width_ + 1;
  }

  std::ostream& operator<<(std::ostream& ostr,
			   const rectangle2d& win)
  {
    ostr << "[width=" << win.width() << ", height=" << win.height() << ']';
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_RECTANGLE2D_HH
