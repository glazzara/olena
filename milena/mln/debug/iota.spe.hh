// Copyright (C) 2007, 2008, 2009, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE).
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

#ifndef MLN_DEBUG_IOTA_SPE_HH
# define MLN_DEBUG_IOTA_SPE_HH

/// \file
/// \brief Specializations of mln::debug::iota.

# ifndef MLN_DEBUG_IOTA_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DEBUG_IOTA_HH

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace debug
  {


    namespace impl
    {

      template <typename I>
      void iota(trait::image::speed::fastest, I& input, unsigned base_index);

# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void
      iota(trait::image::speed::fastest, I& input, unsigned base_index)
      {
	unsigned i = base_index;
	mln_pixter(I) p(input);
        typedef mln_value(I) V;
	for_all(p)
	  p.val() =
          static_cast<V>(++i % mln_max(typename trait::modulus_type<V>::ret));;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::debug::impl

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_IOTA_SPE_HH
