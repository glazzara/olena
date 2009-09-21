// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_GRAPH_MORPHO_MAKE_COMPLEX2D_HH
# define APPS_GRAPH_MORPHO_MAKE_COMPLEX2D_HH

/// \file
/// \brief Cubical 2-complex creation from a 2D image of pixels.

# include <mln/core/image/image2d.hh>

/** \brief Create an binary 2D image representing a cubical 2-complex
    by doubling the resolution of the image \a input images to insert
    edges and squares "between" the pixels of \a input.
   
    The values set on "edge" sites and "square" sites are a
    conjunction of the values on the face of the adjacent faces of
    immediate lower dimension.  */
template <typename I>
inline
mln_concrete(I)
make_complex2d(const mln::Image<I>& input_)
{
  using namespace mln;

  const I& input = exact(input_);
  /* FIXME: The construction of OUTPUT is obvioulsy not generic, since
     it expects I's domain to provide the interface of an mln::box2d.
     There should be a static precondition on I at the beginning of
     this function.  */
  typedef mln_concrete(I) O;
  O output(2 * input.nrows() - 1, 2 * input.ncols() - 1);

  mln_piter(O) p(output.domain());
  for_all(p)
  {
    point2d p_ = p;
    if (p_.row() % 2 == 0)
      {
	if (p_.col() % 2 == 0)
	  // Pixel site.
	  output(p) = input.at_(p_.row() / 2, p_.col() / 2);
	else
	  // Horizontal edge site.
	  output(p) =
	    input.at_(p_.row() / 2, (p_.col() - 1) / 2) &&
	    input.at_(p_.row() / 2, (p_.col() + 1) / 2);
      }
    else
      {
	if (p_.col() % 2 == 0)
	  // Vertical edge site.
	  output(p) =
	    input.at_((p_.row() - 1) / 2, p_.col() / 2) &&
	    input.at_((p_.row() + 1) / 2, p_.col() / 2);
	else
	  // Square site (not handled, but still set for possible
	  // debug outputs).
	  output(p) = 
	    input.at_((p_.row() - 1) / 2, (p_.col() - 1) / 2) &&
	    input.at_((p_.row() - 1) / 2, (p_.col() + 1) / 2) &&
	    input.at_((p_.row() + 1) / 2, (p_.col() - 1) / 2) &&
	    input.at_((p_.row() + 1) / 2, (p_.col() + 1) / 2);
      }
  }
  return output;
}

#endif // ! APPS_GRAPH_MORPHO_MAKE_COMPLEX2D_HH
