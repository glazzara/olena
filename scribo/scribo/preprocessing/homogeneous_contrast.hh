// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PREPROCESSING_HOMOGENEOUS_CONTRAST_HH
# define SCRIBO_PREPROCESSING_HOMOGENEOUS_CONTRAST_HH

/// \file
///
/// Improve contrast homogeneity in an image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/morpho/closing/leveling.hh>

#include <mln/arith/minus.hh>


namespace scribo
{

  namespace preprocessing
  {


    using namespace mln;

    /// \brief Improve contrast homogeneity in an image.
    ///
    /// \param[in] input    A gray-level image.
    /// \param[in] h        Height attribute value for leveling closing.
    ///
    /// \return A gray-level image with better contrast homogeneity.
    //
    template <typename I>
    mln_concrete(I)
    homogeneous_contrast(const Image<I>& input, unsigned h);


    /// \overload
    /// Default height attribute value is set to 75.
    //
    template <typename I>
    mln_concrete(I)
    homogeneous_contrast(const Image<I>& input);


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY


namespace scribo
{

  namespace preprocessing
  {


    template <typename I>
    mln_concrete(I)
    homogeneous_contrast(const Image<I>& input_, unsigned h)
    {
      mln_trace("scribo::preprocessing::homogeneous_contrast");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      morpho::attribute::height<I> acc;
      mln_concrete(I)
	output = morpho::closing::leveling(input, c4(), acc, h);
      arith::minus_inplace(output, input);

      return output;
    }


    template <typename I>
    mln_concrete(I)
    homogeneous_contrast(const Image<I>& input)
    {
      return homogeneous_contrast(input, 75);
    }


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

# endif // SCRIBO_PREPROCESSING_HOMOGENEOUS_CONTRAST_HH
