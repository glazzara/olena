// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH
# define SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH


/// \file
///
/// Localize text in a picture.

# include <scribo/toolchain/internal/text_in_picture_functor.hh>

namespace scribo
{

  namespace toolchain
  {

    using namespace mln;


    template <typename I>
    component_set<mln_ch_value(I, def::lbl_type)>
    text_in_picture(const Image<I>& input_rgb_orig,
		    bool bg_removal, bool multi_scale_bin,
		    unsigned max_dim_size = 0, unsigned lambda = 0,
		    bool verbose = false);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    component_set<mln_ch_value(I, def::lbl_type)>
    text_in_picture(const Image<I>& input_rgb_,
		    bool bg_removal, bool multi_scale_bin,
		    unsigned max_dim_size, unsigned lambda,
		    bool verbose)
    {
      trace::entering("scribo::toolchain::text_in_picture");

      const I& input_rgb = exact(input_rgb_);
      mln_precondition(input_rgb.is_valid());

      internal::text_in_picture_functor<I> f;
      f.enable_bg_removal = bg_removal;
      f.enable_multi_scale_bin = multi_scale_bin;
      f.max_dim_size = max_dim_size;
      f.lambda = lambda;
      f.verbose = verbose;

      component_set<mln_ch_value(I, def::lbl_type)>
	output = f(input_rgb);

      trace::exiting("scribo::toolchain::text_in_picture");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH

