// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_EXTRACT_GREEN_HH
# define MLN_EXTRACT_GREEN_HH

/// \file mln/extract/blue.hh
///
/// Extract the blue component of an image.


# include <mln/core/image/fun_image.hh>
# include <mln/fun/meta/blue.hh>

namespace mln
{

  namespace extract
  {

    template <typename I>
    fun_image<meta::blue<mln_value(I)>,I>
    blue(Image<I>& ima);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    fun_image<meta::blue<mln_value(I)>,I>
    blue(Image<I>& ima)
    {
      fun_image<meta::blue<mln_value(I)>,I> result(ima);
      return result;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extract

} // end of namespace mln

#endif // ! MLN_EXTRACT_GREEN_HH
