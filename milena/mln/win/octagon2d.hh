// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file mln/win/octagon2d.hh
///
/// Definition of the mln::win::octagon2d window.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  mln_internal_add_classical_window_trait(octagon2d);


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
     * \verbatim
     *     o o o
     *   o o o o o
     * o o o o o o o
     * o o o x o o o
     * o o o o o o o
     *   o o o o o
     *     o o o
     * \endverbatim
     * is defined with L = 7 (l = 1).
     *
     * \ingroup modwin2d
     */
    struct octagon2d : public internal::classical_window_base< dpoint2d, octagon2d >
    {
      /*! \brief Constructor.
       *
       * \param[in] length Length, of the octagon.
       *
       * \pre \p length is such as length = 6*x + 1 where x >= 0.
       */
      octagon2d(unsigned length);

      /*! \brief Give the octagon length, that is, its width.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta_() const;

      /*! \brief Give the area.
       */
      unsigned area() const;

      void print_(std::ostream& ostr) const;

    protected:
      unsigned length_;
    };



# ifndef MLN_INCLUDE_ONLY

    inline
    octagon2d::octagon2d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 6 == 1);
      const def::coord
	y = static_cast<def::coord>(length / 6),
	x = static_cast<def::coord>(y * 2),
	z = static_cast<def::coord>(y + x);
      insert(dpoint2d(0, 0));
      for (def::coord a = 1; a <= x; ++a)
	for (def::coord b = 0; b <= x; ++b)
	{
	  def::coord 
	    _a = static_cast<def::coord>(-a),
	    _b = static_cast<def::coord>(-b);
	  insert(dpoint2d(a, b));
	  insert(dpoint2d(_b, a));
	  insert(dpoint2d(b, _a));
	  insert(dpoint2d(_a, _b));
	}
      const def::coord a_min = static_cast<def::coord>(x + 1);
      for (def::coord a = a_min; a <= z; ++a)
	{
	  const def::coord
	    b_min = static_cast<def::coord>(-2 * x + a),
	    b_max = static_cast<def::coord>(2 * x - a);
	  for (def::coord b = b_min; b <= b_max; ++b)
	    {
	      def::coord 
		_a = static_cast<def::coord>(-a),
		_b = static_cast<def::coord>(-b);
	      insert(dpoint2d(a, b));
	      insert(dpoint2d(a, _b));
	      insert(dpoint2d(_a, b));
	      insert(dpoint2d(_a, _b));
	      insert(dpoint2d(b, a));
	      insert(dpoint2d(b, _a));
	      insert(dpoint2d(_b, a));
	      insert(dpoint2d(_b, _a));
	    }
	}
    }

    inline
    unsigned octagon2d::length() const
    {
      return length_;
    }

    inline
    unsigned octagon2d::delta_() const
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
    void octagon2d::print_(std::ostream& ostr) const
    {
      ostr << "[octagon2d: length=" << length_ << ']';
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
