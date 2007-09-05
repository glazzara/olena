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

#ifndef MLN_CORE_WIN_SEGMENT1D_HH
# define MLN_CORE_WIN_SEGMENT1D_HH

/*! \file mln/core/win/segment1d.hh
 *
 * \brief Definition of the mln::win::segment1d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/dpoint1d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{

  namespace win
  {

    /*! \brief Segment window defined on the 1D grid.
     *
     * An segment1d is centered and symmetrical; so
     * its height (length) is odd.
     *
     * For instance: \n
     *  o x o \n
     * is defined with length = 3.
     */
    struct segment1d : public Window< segment1d >,
		     public internal::dpoints_base_< dpoint1d, segment1d >
    {
      /// Point associated type.
      typedef point1d point;

      /// Dpoint associated type.
      typedef dpoint1d dpoint;

      /*! \brief Point_Iterator type to browse a segment such as: "for each row
       * (increasing), for each column (increasing)."
       */
      typedef dpoints_fwd_piter<dpoint1d> fwd_qiter;

      /*! \brief Point_Iterator type to browse a segment such as: "for each row
       * (decreasing), for each column (decreasing)."
       */
      typedef dpoints_bkd_piter<dpoint1d> bkd_qiter;

      /*! \brief Same as fwd_qiter.
       */
      typedef fwd_qiter qiter;

      /*! \brief Constructor.
       *
       * \param[in] length Length, thus height, of the segment1d.
       *
       * \pre \p length is odd.
       */
      segment1d(unsigned length);

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

      /*! \brief Give the segment length, that is, its height.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Apply a central symmetry to the target window.
      segment1d& sym();

    protected:
      unsigned length_;
    };


    /*! \brief Print a segment1D window \p win into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win A segment1D window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::win::segment1d
     */
    std::ostream& operator<<(std::ostream& ostr, const segment1d& win);



# ifndef MLN_INCLUDE_ONLY

    segment1d::segment1d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 2 == 1);
      const int dind = length / 2;
      for (int ind = - dind; ind <= dind; ++ind)
	insert(make::dpoint1d(ind));
    }

    bool segment1d::is_centered() const
    {
      return true;
    }

    bool segment1d::is_symmetric() const
    {
      return true;
    }

    unsigned segment1d::length() const
    {
      return length_;
    }

    unsigned segment1d::delta() const
    {
      return length_ / 2;
    }

    segment1d& segment1d::sym()
    {
      return *this;
    }

    std::ostream& operator<<(std::ostream& ostr, const segment1d& win)
    {
      ostr << "[segment1d: length=" << win.length() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_CORE_WIN_SEGMENT1D_HH
