// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_CORE_INIT_INTEGRAL_IMAGE_HH
# define SCRIBO_CORE_INIT_INTEGRAL_IMAGE_HH

/// \file
///
///

# include <mln/core/image/image2d.hh>

namespace scribo
{
  using namespace mln;


  template <typename I, typename F>
  mln_ch_value(I,double)
  init_integral_image(const Image<I>& input_, F& func);


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    inline
    double square_(const double& val)
    {
      double v = static_cast<double>(val);
      return v * v;
    }

    inline
    double identity_(const double& val)
    {
      return static_cast<double>(val);
    }

  } // end of namespace scribo::internal


  // Facade

  template <typename I, typename F>
  mln_ch_value(I,double)
  init_integral_image(const Image<I>& input_, F& func)
  {
    mln_trace("scribo::init_integral_image");

    const I& input = exact(input_);
    mln_precondition(input.is_valid());
    mln_precondition(input.domain().pmin() == literal::origin);

    mln_ch_value(I,double) output;
    initialize(output, input);

    unsigned
      nrows_ = input.nrows(),
      ncols_ = input.ncols();

    output.at_(0,0) = func(input.at_(0, 0));

    for (unsigned row = 1; row < nrows_; ++row)
      output.at_(row,0) = output.at_(row - 1, 0) + func(input.at_(row, 0));

    for (unsigned col = 1; col < ncols_; ++col)
      output.at_(0,col) = output.at_(0, col - 1)
	+ func(input.at_(0, col));

    for (unsigned row = 1; row < nrows_; ++row)
      for (unsigned col = 1; col < ncols_; ++col)
	output.at_(row, col) = output.at_(row - 1, col)
	  + output.at_(row, col - 1)
	  - output.at_(row - 1, col - 1)
	  + func(input.at_(row, col));

    return output;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_INIT_INTEGRAL_IMAGE_HH
