// Copyright (C) 2006  EPITA Research and Development Laboratory
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


#ifndef MLN_MAKE_WIN_CHAMFER_HH
# define MLN_MAKE_WIN_CHAMFER_HH\

/*!
 * \file   mln/make/win_chamfer.hh
 * 
 * \brief  Routine to create chamfer mln::w_window2d_int.
 *
 */


# include <mln/core/alias/w_window2d_int.hh>
# include <mln/core/alias/w_window2d_float.hh>
# include <math.h>

namespace mln
{

  namespace make
  {

    template<int d10, int d11>
    const mln::w_window2d_int
    mk_chamfer_3x3_int()
    {
      int ws[] = { d11, d10, d11,
		   d10,   0,   0,
		     0,   0,   0 };

      return (make::w_window2d(ws));
    }

    template<int d10, int d11, int d21>
    const mln::w_window2d_int
    mk_chamfer_5x5_int()
    {
      int ws[] = {   0, d21,   0, d21,   0,
		   d21, d11, d10, d11, d21,
		     0, d10,   0,   0,   0,
		     0,   0,   0,   0,   0,
		     0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const mln::w_window2d_float
    mk_chamfer_3x3_float(float d10, float d11)
    {
      float ws[] = { d11, d10, d11,
		     d10,   0,   0,
		       0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const mln::w_window2d_float
    mk_chamfer_5x5_float(float d10, float d11, float d21)
    {
      float ws[] = {   0, d21,   0, d21,   0,
		     d21, d11, d10, d11, d21,
		       0, d10,   0,   0,   0,
		       0,   0,   0,   0,   0,
		       0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const mln::w_window2d_float
    mk_chamfer_exact()
    {
      float r2 = sqrt(2);
      float ws[] = { r2, 1, r2,
		      1, 0,  0,
		      0, 0,  0 };

      return (make::w_window2d(ws));
    }

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_WIN_CHAMFER_HH
