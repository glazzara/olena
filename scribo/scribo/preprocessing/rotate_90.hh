// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PREPROCESSING_ROTATE_90_HH
# define SCRIBO_PREPROCESSING_ROTATE_90_HH

/// \file
///
/// Fast +90/-90 degrees rotation.


# include <mln/core/concept/image.hh>
# include <mln/geom/all.hh>

// FIXME: not generic
# include <mln/core/alias/dpoint2d.hh>

//FIXME: add more preconditions + a dispatch !?

namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    /*! \brief Perform a +90/-90 degrees rotation.

      \param[in] input An image.
      \param[in] positive If set to true, performs a +90° rotation,
                          -90° otherwise.

      \return A rotated image.
    */
    template <typename I>
    mln_concrete(I)
    rotate_90(const Image<I>& input, bool positive);

    /// \brief Performs a +90° rotation.
    /// \overload
    //
    template <typename I>
    mln_concrete(I)
    rotate_90(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    rotate_90(const Image<I>& input_, bool positive)
    {
      trace::entering("scribo::preprocessing::rotate_90");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      // FIXME: Does this routine accept images with an origin
      // different from (0,0) and with negative coordinates?
      //
      // mln_precondition(input.domain().pmin().row() > 0);
      // mln_precondition(input.domain().pmin().col() > 0);

      // Works only on one block images.
      mlc_is(mln_trait_image_value_access(I),
	     mln::trait::image::value_access::direct)::check();
      mlc_is(mln_trait_image_value_storage(I),
	     mln::trait::image::value_storage::one_block)::check();

      // Initialize output domain and make sure the border size is the
      // same as input's.
      typedef mln_domain(I) domain_t;
      mln_site(I) pmax(input.domain().pmax().col(),
		       input.domain().pmax().row());
      mln_concrete(I) output(domain_t(input.domain().pmin(), pmax),
			     input.border());

      mln_assertion(output.is_valid());

      const mln_value(I)* in_ptr = input.buffer();

      unsigned
	in_ncols = geom::ncols(input) + 2 * input.border(),
	in_nrows = geom::nrows(input) + 2 * input.border();

      unsigned
	out_ncols = geom::ncols(output) + 2 * input.border(),
	out_nrows = geom::nrows(output) + 2 * input.border();

      int
	out_next_offset,
	out_next_p_offset;

      mln_value(I)* out_ptr = output.buffer();

      if (positive) // +90 deg
      {
	dpoint2d dp(out_nrows, + 1);
	out_next_offset = output.delta_index(dp);

	out_next_p_offset = output.delta_index(dpoint2d(-1, 0));

	out_ptr += output.delta_index(dpoint2d(out_nrows - 1, 0));
      }
      else // -90 deg
      {
	dpoint2d dp(- out_nrows, - 1);
	out_next_offset = output.delta_index(dp);

	out_next_p_offset = output.delta_index(dpoint2d(+1, 0));

	out_ptr += out_ncols - 1;
      }

      for (unsigned row = 0; row < in_nrows; ++row)
      {
	for (unsigned col = 0; col < in_ncols;
	     ++col, ++in_ptr, out_ptr += out_next_p_offset)
	  *out_ptr = *in_ptr;

	out_ptr += out_next_offset;
      }

      trace::exiting("scribo::preprocessing::rotate_90");
      return output;
    }


    template <typename I>
    mln_concrete(I)
    rotate_90(const Image<I>& input)
    {
      return rotate_90(input, true);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# endif // SCRIBO_PREPROCESSING_ROTATE_90_HH

