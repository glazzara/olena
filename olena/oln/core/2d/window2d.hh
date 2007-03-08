// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_2D_WINDOW2D_HH
# define OLN_CORE_2D_WINDOW2D_HH

# include <oln/core/gen/window.hh>
# include <oln/core/2d/dpoint2d.hh>
# include <oln/core/2d/aliases.hh>


namespace oln
{

  // FIXME: window2d should be an actual type, not an alias...


  window2d mk_square(unsigned odd_len);
  


# ifndef OLN_INCLUDE_ONLY

  window2d mk_square(unsigned odd_len)
  {
    precondition(odd_len % 2 == 1);
    window2d tmp;
    int half_len = odd_len / 2;
    for (int drow = - half_len; drow <= half_len; ++drow)
      for (int dcol = - half_len; dcol <= half_len; ++dcol)
	tmp.take(dpoint2d(drow, dcol));
    return tmp;
  }

# endif


  extern const window2d win3x3;
  extern const window2d win5x5;


# ifndef OLN_INCLUDE_ONLY

  const window2d win3x3 = mk_square(3);
  const window2d win5x5 = mk_square(5);

# endif


} // end of namespace oln


#endif // ! OLN_CORE_2D_WINDOW2D_HH
