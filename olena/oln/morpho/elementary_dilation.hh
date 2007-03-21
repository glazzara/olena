// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef	OLN_MORPHO_ELEMENTARY_DILATION_HH
# define OLN_MORPHO_ELEMENTARY_DILATION_HH

#include <oln/level/apply_local.hh>
#include <oln/border/fill.hh>
#include <oln/accumulator/max.hh>
#include <oln/accumulator/or.hh>

namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I>
    oln_plain(I)
    elementary_dilation(const Image_with_Nbh<I>& input);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I>
      oln_plain(I)
      elementary_dilation_on_function_(const Image<I>&,
				       const I& input)
      {
	border::fill(input, oln_min(oln_value(I)));
	accumulator::max_<oln_value(I)> max;
	return level::apply_local(max, input);
      }

      template <typename I>
      oln_plain(I)
      elementary_dilation_on_set_(const Image<I>&,
				  const I& input)
      {
	border::fill(input, oln_min(oln_value(I)));
	accumulator::or_<oln_value(I)> accu_or;
	return level::apply_local(accu_or, input);
      }

      // FIXME: Add a fast version.


      // Impl facade.

      template <typename I>
      oln_plain(I) elementary_dilation_(const Image<I>& input)
      {
	return elementary_dilation_on_function_(exact(input), exact(input));
      }

      template <typename I>
      oln_plain(I) elementary_dilation_(const Binary_Image<I>& input)
      {
	return elementary_dilation_on_set_(exact(input), exact(input));
      }

    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I>
    oln_plain(I)
    elementary_dilation(const Image_with_Nbh<I>& input)
    {
      return impl::elementary_dilation_(exact(input));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_ELEMENTARY_DILATION_HH
