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

#ifndef MLN_CORE_WIN_DISK2D_HH
# define MLN_CORE_WIN_DISK2D_HH

/*! \file mln/core/win/disk2d.hh
 *
 * \brief Definition of the mln::win::disk2d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{

  namespace win
  {
 
    /*! \brief Disk window defined on the 2D square grid.
     *
     * An disk2d is centered and symmetrical.
     *
     */
    struct disk2d : public Window< disk2d >,
		    public internal::dpoints_base_< dpoint2d, disk2d >
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
       * \param[in] length Length, thus diameter.
       *
       */
      disk2d(unsigned length);

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

      /*! \brief Give the disk length, that is, its width.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Apply a central symmetry to the target window.
      disk2d& sym();

    protected:
      unsigned length_;
    };


    /*! \brief Print an disk window \p win into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win A disk window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::win::disk2d
     */
    std::ostream& operator<<(std::ostream& ostr, const disk2d& win);

 

# ifndef MLN_INCLUDE_ONLY

    disk2d::disk2d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 2 == 1);
      const int r = length / 2;
      const int r2 = r * r;
      for (int a = -r; a <= r; ++a)
	for (int b = -r; b <= r; ++b)
	  if (a * a + b * b <= r2)
	    insert(make::dpoint2d(a, b));
    }

    bool disk2d::is_centered() const
    {
      return true;
    }

    bool disk2d::is_symmetric() const
    {
      return true;
    }

    unsigned disk2d::length() const
    {
      return length_;
    }

    unsigned disk2d::delta() const
    {
      return length_ / 2;
    }

    disk2d& disk2d::sym()
    {
      return *this;
    }

    std::ostream& operator<<(std::ostream& ostr, const disk2d& win)
    {
      ostr << "[disk2d: length=" << win.length() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_CORE_WIN_DISK2D_HH
