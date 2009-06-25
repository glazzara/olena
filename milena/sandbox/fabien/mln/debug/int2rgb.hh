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

#ifndef MLN_DEBUG_INT2RGB_HH
# define MLN_DEBUG_INT2RGB_HH

/// \file
///
/// FIXME

#include <mln/core/concept/image.hh>
#include <mln/value/rgb8.hh>


namespace mln
{

  namespace debug
  {

    /// \brief FIXME
    ///
    /// \param[in] input The input image.
    /// \return An image which values are cut into rgb channels.
    /// It allows visualization of integer values superior to 256.
    ///
    template <typename I>
    inline
    mln_ch_value(I, value::rgb8)
    int2rgb(const Image<I>& input);


#ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_ch_value(I, value::rgb8)
    int2rgb(const Image<I>& input_)
    {
      trace::entering("debug::int2rgb");

      const I& input = exact(input_);

      mln_ch_value(I, value::rgb8) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      for_all(p)
      {
	unsigned value = input(p);
	output(p).blue() = value % 256;
	value /= 256;
	output(p).green() = value % 256;
	value /= 256;
	output(p).red() = value % 256;
      }

      trace::exiting("debug::int2rgb");
      return output;
    }

#endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::debug

} // end of namespace mln

#endif // ! MLN_DEBUG_INT2RGB_HH
