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

#ifndef CLASSIFY_WITH_HISTO_3D_HH_
# define CLASSIFY_WITH_HISTO_3D_HH_


namespace mln
{

  namespace histo
  {

    template <typename T, typename C>
    image2d<C>
    classify_with_histo_3d(image2d<T>& ref,
			   image3d<C>& regions)
    {
      image2d<C> out;
      initialize(out, ref);

      mln_fwd_piter(image2d<T>) p(ref.domain());
      for_all(p)
      {
	// get 3d point in regions image.
	point3d p3 = point3d(ref(p).comp(0), ref(p).comp(1), ref(p).comp(2));
	out(p) = regions(p3); // copy the label in out's pixel.
      }

      return out;
    }

  } // end of namespace mln::histo

} // end of namespace mln

#endif /* !CLASSIFY_WITH_HISTO_3D_HH_ */
