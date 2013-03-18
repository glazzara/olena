// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_DATA_APPLY_SPE_HH
# define MLN_DATA_APPLY_SPE_HH

/// \file
///
/// \brief Specializations for mln::data::apply.

# ifndef MLN_DATA_APPLY_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DATA_APPLY_HH

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
	mln_trace("data::impl::apply_");

	mln_pixter(I) pxl(input);
	for_all(pxl)
	  pxl.val() = f(pxl.val());

      }


    } // end of namespace mln::data::impl

  } // end of namespace mln::data

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_APPLY_SPE_HH
