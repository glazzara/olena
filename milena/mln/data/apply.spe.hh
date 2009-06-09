// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DATA_APPLY_SPE_HH
# define MLN_DATA_APPLY_SPE_HH

/*! \file
 *
 * \brief Specializations for mln::data::apply.
 */

# ifndef MLN_LEVEL_APPLY_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_APPLY_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace data
  {

    namespace impl
    {

      namespace generic
      {
	template <typename I, typename F>
	void apply_(I& input, const F& f);
      }


      // Disjunction.

      template <typename I, typename F>
      inline
      void apply_(trait::image::speed::any, I& input, const F& f)
      {
	generic::apply_(input, f);
      }

      template <typename I, typename F>
      inline
      void apply_(trait::image::speed::fastest, I& input, const F& f)
      {
	trace::entering("data::impl::apply_");

	mln_pixter(I) pxl(input);
	for_all(pxl)
	  pxl.val() = f(pxl.val());

	trace::exiting("data::impl::apply_");
      }


    } // end of namespace mln::data::impl

  } // end of namespace mln::data

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_APPLY_SPE_HH
