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

/*! \file ?/sub_image.hh
 *
 * \brief .
 */

#include <mln/core/image_if_value.hh>
#include <mln/core/image2d_b.hh>
#include <mln/value/rgb8.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>


#include <mln/trait/image_from_mesh.hh>

namespace mln
{

  template <typename I>
  typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
  color (const Image<I>& input_)
  {
    const I& input = exact (input_);

    image2d_b<value::rgb8> output(input.domain().bbox());
    data::fill(output, value::rgb8(255, 0, 0));

//     /// FIXME by :
//     data::paste(input, output);
    {
      mln_piter(I) p(input.domain());

      for_all(p)
	output(p) = value::rgb8(input(p));
    }
    return output;
  }
}
