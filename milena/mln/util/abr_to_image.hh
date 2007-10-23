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

#ifndef MLN_UTIL_ABR_TO_IMAGE_HH
# define MLN_UTIL_ABR_TO_IMAGE_HH

/*!
 *  \file   mln/util/abr_to_image.hh
 *
 *  \brief Definition of function which transform a tree into an
 *  image.
 *
 */

#include <mln/util/abr.hh>

namespace mln
{

  namespace util
  {

    template <typename T, typename I>
    void
    abr_to_image_rec(abr<T>& abr, Image<I>& output_, const mln_value(I) lvl);

    template <typename T, typename I>
    void
    abr_to_image (abr<T>& abr, Image<I>& output_);

# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename I>
    void
    abr_to_image_rec(abr<T>& abr, Image<I>& output_, const mln_value(I) lvl)
    {
      I& output = exact(output_);

      mln_piter(T) p(abr.elt_);

      for_all(p)
	output(p) = lvl;

       typename std::vector< util::abr<T>* >::const_iterator it = abr.child_.begin();
      for (;
	   it != abr.child_.end();
	   ++it)
 	abr_to_image_rec((**it), output, lvl + 1);
    }


    template <typename T, typename I>
    void
    abr_to_image (abr<T>& abr, Image<I>& output_)
    {
      I& output = exact(output_);
      abr_to_image_rec(abr, output, 1);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // !MLN_UTIL_ABR_TO_IMAGE_HH
