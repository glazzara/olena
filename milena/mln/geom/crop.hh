// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// Crop an image.

#ifndef MLN_GEOM_CROP_HH
# define MLN_GEOM_CROP_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/data/paste.hh>

namespace mln
{

  namespace geom
  {

    template <typename I, typename B>
    mln_concrete(I) crop(const Image<I>& input, const Box<B>& b);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename B>
    mln_concrete(I) crop(const Image<I>& input_, const Box<B>& b_)
    {
      trace::entering("mln::geom::crop");

      const I& input = exact(input_);
      const B& b = exact(b_);
      mln_precondition(input.is_valid());
      mln_precondition(b.is_valid());
      mln_precondition(b <= input.domain());

      mln_concrete(I) output(b);
      data::paste(input | b, output);

      trace::exiting("mln::geom::crop");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln

#endif // ! MLN_GEOM_CROP_HH
