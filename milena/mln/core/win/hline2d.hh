// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_WIN_HLINE2D_HH
# define MLN_CORE_WIN_HLINE2D_HH

/*! \file mln/core/win/hline2d.hh
 *
 * \brief Definition of the mln::win::hline2d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{

  namespace win
  {
 
    /*! \brief Horizontal line window defined on the 2D square grid.
     *
     * An hline2d is centered and symmetrical; so its height is 1 and
     * its width (length) is odd.
     *
     * For instance: \n
     *  o o x o o \n
     * is defined with length = 5.
     */
    struct hline2d : public Window< hline2d >,
		     public mln::internal::set_of_<dpoint2d>
    {
      /// Point associated type.
      typedef point2d point;

      /// Dpoint associated type.
      typedef dpoint2d dpoint;

      /*! \brief Point_Iterator type to browse a hline such as: "for each row
       * (increasing), for each column (increasing)."
       */
      typedef dpoints_fwd_piter<dpoint2d> fwd_qiter;

      /*! \brief Point_Iterator type to browse a hline such as: "for each row
       * (decreasing), for each column (decreasing)."
       */
      typedef dpoints_bkd_piter<dpoint2d> bkd_qiter;

      /*! \brief Same as fwd_qiter.
       */
      typedef fwd_qiter qiter;

      /*! \brief Constructor.
       *
       * \param[in] length Length, thus width, of the horizontal line.
       *
       * \pre \p length is odd.
       */
      hline2d(unsigned length);

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

      /*! \brief Give the hline length, that is, its width.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Get the symmetrical window.
      hline2d sym_() const;

    protected:
      unsigned length_;
    };


    /*! \brief Print an horizontal 2D line window \p win into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win An horizontal 2D line window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::hline2d
     */
    std::ostream& operator<<(std::ostream& ostr, const hline2d& win);

 

# ifndef MLN_INCLUDE_ONLY

    hline2d::hline2d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 2 == 1);
      const int dcol = length / 2;
      for (int col = - dcol; col <= dcol; ++col)
	insert(make::dpoint2d(0, col));
    }

    bool hline2d::is_centered() const
    {
      return true;
    }

    bool hline2d::is_symmetric() const
    {
      return true;
    }

    unsigned hline2d::length() const
    {
      return length_;
    }

    unsigned hline2d::delta() const
    {
      return length_ / 2;
    }

    hline2d hline2d::sym_() const
    {
      return *this;
    }

    std::ostream& operator<<(std::ostream& ostr, const hline2d& win)
    {
      ostr << "[line2d: length=" << win.length() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_CORE_WIN_HLINE2D_HH
