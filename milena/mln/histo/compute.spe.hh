// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_HISTO_COMPUTE_SPE_HH
# define MLN_HISTO_COMPUTE_SPE_HH

/*! \file mln/histo/compute.spe.hh
 *
 * \brief Specializations of histogram computation.
 */

# ifndef MLN_HISTO_COMPUTE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_HISTO_COMPUTE_HH


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace histo
  {

    namespace impl
    {

      namespace generic
      {
	template <typename I>
	array<mln_value(I)> compute_(const I& input);
      }

      template <typename I>
      inline
      array<mln_value(I)>
      compute_(trait::image::speed::any, const I& input)
      {
	return generic::compute_(input);
      }

      template <typename I>
      inline
      array<mln_value(I)>
      compute_(trait::image::speed::fastest, const I& input)
      {
	array<mln_value(I)> h;
	mln_pixter(const I) p(input);
	for_all(p)
	  ++h(p.val());
	return h;
      }

    } // end of namespace mln::histo::impl

  } // end of namespace mln::histo

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_HISTO_COMPUTE_SPE_HH
