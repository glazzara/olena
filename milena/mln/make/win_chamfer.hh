// Copyright (C) 2006, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MAKE_WIN_CHAMFER_HH
# define MLN_MAKE_WIN_CHAMFER_HH

/// \file
///
/// \brief Routine to create chamfer mln::w_window2d_int.
///
/// \todo Revamp!

# include <cmath>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/core/alias/w_window2d_float.hh>


namespace mln
{

  namespace make
  {

    template<int d10, int d11>
    mln::w_window2d_int
    mk_chamfer_3x3_int();

    template<int d10, int d11, int d21>
    mln::w_window2d_int
    mk_chamfer_5x5_int();

    mln::w_window2d_float
    mk_chamfer_3x3_float(float d10, float d11);

    mln::w_window2d_float
    mk_chamfer_5x5_float(float d10, float d11, float d21);

    mln::w_window2d_float
    mk_chamfer_exact();


# ifndef MLN_INCLUDE_ONLY

    template<int d10, int d11>
    inline
    mln::w_window2d_int
    mk_chamfer_3x3_int()
    {
      int ws[] = { d11, d10, d11,
		   d10,   0,   0,
		     0,   0,   0 };

      return make::w_window2d(ws);
    }

    template<int d10, int d11, int d21>
    inline
    mln::w_window2d_int
    mk_chamfer_5x5_int()
    {
      int ws[] = {   0, d21,   0, d21,   0,
		   d21, d11, d10, d11, d21,
		     0, d10,   0,   0,   0,
		     0,   0,   0,   0,   0,
		     0,   0,   0,   0,   0 };

      return make::w_window2d(ws);
    }

    inline
    mln::w_window2d_float
    mk_chamfer_3x3_float(float d10, float d11)
    {
      float ws[] = { d11, d10, d11,
		     d10,   0,   0,
		       0,   0,   0 };

      return make::w_window2d(ws);
    }

    inline
    mln::w_window2d_float
    mk_chamfer_5x5_float(float d10, float d11, float d21)
    {
      float ws[] = {   0, d21,   0, d21,   0,
		     d21, d11, d10, d11, d21,
		       0, d10,   0,   0,   0,
		       0,   0,   0,   0,   0,
		       0,   0,   0,   0,   0 };

      return make::w_window2d(ws);
    }

    inline
    mln::w_window2d_float
    mk_chamfer_exact()
    {
      float r2 = std::sqrt(2.0f);
      float ws[] = { r2, 1, r2,
		      1, 0,  0,
		      0, 0,  0 };

      return make::w_window2d(ws);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_WIN_CHAMFER_HH
