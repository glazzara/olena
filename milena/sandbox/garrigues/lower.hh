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


#ifndef MLN_FIXME_FLLT_LOWER_HH
# define MLN_FIXME_FLLT_LOWER_HH

/*! \file fllt.hh
 *
 * \brief Informations about how to compute the lower level set.
 *
 */

# include <mln/core/alias/neighb2d.hh>

# include <mln/accu/min.hh>
# include <mln/labeling/regional_minima.hh>

# include "fllt_types.hh"

namespace mln
{
  namespace fllt
  {

    //Fwd declaration.
    template <typename V> struct upper;

    //   LOWER LEVEL SET : region = c4, border = c8
    template <typename V>
    struct lower
    {
      typedef upper<V> opposite;
      typedef lower_t tag;
      static bool
      compare(const V& u, const V& v)
      {
	return u < v;
      }

      template <typename I, typename N, typename O>
      static bool
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh,
			Image<O>& output, unsigned& nlabels)
      {
	return labeling::regional_minima(input, nbh,
					 output, nlabels);
      }

      static const int inc = 1;
      static const bool parent_is_brighter = true;
      typedef accu::min accu_for_gn;

      static const neighb2d& bdr_nbh() { return c8(); }
      static const neighb2d& reg_nbh() { return c4(); }

    };

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_LOWER_HH
