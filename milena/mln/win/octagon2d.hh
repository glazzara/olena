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

#ifndef MLN_WIN_OCTAGON2D_HH
# define MLN_WIN_OCTAGON2D_HH

/*! \file mln/win/octagon2d.hh
 *
 * \brief Definition of the mln::win::octagon2d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{

  namespace win
  {

    /*! \brief Octagon window defined on the 2D square grid.
     *
     * An octagon2d is centered and symmetric.
     *
     * The length L of the octagon is such as L = 6 * l + 1
     * where l >= 0.
     *
     * For instance: \n
     *     o o o \n
     *   o o o o o \n
     * o o o o o o o \n
     * o o o x o o o \n
     * o o o o o o o \n
     *   o o o o o \n
     *     o o o \n
     * is defined with L = 7 (l = 1).
     */
    struct octagon2d : public Window< octagon2d >,
		       public internal::dpoints_base_< dpoint2d, octagon2d >
    {
      /// Point associated type.
      typedef point2d point;

      /// Dpoint associated type.
      typedef dpoint2d dpoint;

      /*! \brief Site_Iterator type to browse a hline such as: "for each row
       * (increasing), for each column (increasing)."
       */
      typedef dpoints_fwd_piter<dpoint2d> fwd_qiter;

      /*! \brief Site_Iterator type to browse a hline such as: "for each row
       * (decreasing), for each column (decreasing)."
       */
      typedef dpoints_bkd_piter<dpoint2d> bkd_qiter;

      /*! \brief Same as fwd_qiter.
       */
      typedef fwd_qiter qiter;

      /*! \brief Constructor.
       *
       * \param[in] length Length, of the octagon.
       *
       * \pre \p length is such as length = 6*x + 1 where x >= 0.
       */
      octagon2d(unsigned length);

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

      /*! \brief Give the octagon length, that is, its width.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /*! \brief Give the area.
       */
      unsigned area() const;

      /// Apply a central symmetry to the target window.
      octagon2d& sym();

    protected:
      unsigned length_;
    };


    /*! \brief Print an octagon window \p win into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win An octagon window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::win::octagon2d
     */
    std::ostream& operator<<(std::ostream& ostr, const octagon2d& win);



# ifndef MLN_INCLUDE_ONLY

    inline
    octagon2d::octagon2d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 6 == 1);
      const int y = length / 6;
      const int x = y * 2;
      const int z = y + x;
      insert(dpoint2d(0, 0));
      for (int a = 1; a <= x; ++a)
	for (int b = 0; b <= x; ++b)
	{
	  insert(dpoint2d(a, b));
	  insert(dpoint2d(-b, a));
	  insert(dpoint2d(b, -a));
	  insert(dpoint2d(-a, -b));
	}
      for (int a = x + 1; a <= z; ++a)
	for (int b = -2 * x + a; b <= 2 * x - a; ++b)
	{
	  insert(dpoint2d(a, b));
	  insert(dpoint2d(a, -b));
	  insert(dpoint2d(-a, b));
	  insert(dpoint2d(-a, -b));
	  insert(dpoint2d(b, a));
	  insert(dpoint2d(b, -a));
	  insert(dpoint2d(-b, a));
	  insert(dpoint2d(-b, -a));
	}
    }

    inline
    bool octagon2d::is_centered() const
    {
      return true;
    }

    inline
    bool octagon2d::is_symmetric() const
    {
      return true;
    }

    inline
    unsigned octagon2d::length() const
    {
      return length_;
    }

    inline
    unsigned octagon2d::delta() const
    {
      return length_ / 2;
    }

    inline
    unsigned octagon2d::area() const
    {
      unsigned l = (length_ - 1) / 6;
      return l * (28 * l + 8) + 4;
    }

    inline
    octagon2d& octagon2d::sym()
    {
      return *this;
    }

    inline
    std::ostream& operator<<(std::ostream& ostr, const octagon2d& win)
    {
      ostr << "[octagon2d: length=" << win.length() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



// When rectangle2d is involved, one surely also wants:
# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>
# include <mln/win/diag2d.hh>
# include <mln/win/backdiag2d.hh>

#endif // ! MLN_WIN_OCTAGON2D_HH
