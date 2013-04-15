// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DATA_SPLIT_HH
# define MLN_DATA_SPLIT_HH

# include <mln/core/concept/image.hh>
# include <mln/value/rgb8.hh>

/// \file
///
/// Split a color RGB image into 3 images.
///
/// \todo interface not enough generic!

namespace mln
{

  namespace data
  {

    /*! \brief Split a color RGB image into 3 images.
     *
     * \ingroup mlndata
     */
    template <typename I, typename J>
    void
    split(const Image<I>& input, Image<J>& r, Image<J>& g, Image<J>& b);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename J>
      void
      split(const Image<I>& input_, Image<J>& r_, Image<J>& g_, Image<J>& b_)
      {
	mln_trace("mln::data::split");

	const I& input = exact(input_);
	J& r = exact(r_);
	J& g = exact(g_);
	J& b = exact(b_);

	mln_assertion(input.is_valid());
	mlc_is(mln_value(I), value::rgb8)::check();
	// FIXME: add more checks.
	mlc_is(mln_trait_image_value_storage(I),
	       trait::image::value_storage::one_block)::check();

	initialize(r, input);
	initialize(g, input);
	initialize(b, input);

	typedef const mln_value(I)* in_ptr_t;
	typedef mln_value(J)* out_ptr_t;

	in_ptr_t in_ptr = input.buffer();
	out_ptr_t
	  r_ptr = r.buffer(),
	  g_ptr = g.buffer(),
	  b_ptr = b.buffer();

	for (unsigned n = 0; n < input.nelements(); ++n, ++in_ptr)
	{
	  *r_ptr++ = in_ptr->red();
	  *g_ptr++ = in_ptr->green();
	  *b_ptr++ = in_ptr->blue();
	}

      }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln

#endif // ! MLN_DATA_SPLIT_HH
