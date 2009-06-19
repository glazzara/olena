// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef COMPUTE_HISTO_3D_HH_
# define COMPUTE_HISTO_3D_HH_


#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/trait/value/comp.hh>


namespace mln
{
  namespace histo
  {

    template <typename T>
    inline
    image3d<unsigned>
    compute_histo_3d(const image2d<T>& ima)
    {
      typedef mln_trait_value_comp(T, 0)::enc enc_0; // R -> int_u8
      typedef mln_trait_value_comp(T, 1)::enc enc_1; // G -> int_u8
      typedef mln_trait_value_comp(T, 2)::enc enc_2; // B -> int_u8

      image3d<unsigned> out(box3d(point3d(mln_min(enc_0),    // -> 0
					  mln_min(enc_1),    // -> 0
					  mln_min(enc_2)),   // -> 0
				  point3d(mln_max(enc_0),    // -> 255
					  mln_max(enc_1),    // -> 255
					  mln_max(enc_2)))); // -> 255
      data::fill(out, 0);

      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point3d(ima(p).comp(0), ima(p).comp(1), ima(p).comp(2)));

      return out;
    }

  }
}

#endif /* !COMPUTE_HISTO_3D_HH_ */
