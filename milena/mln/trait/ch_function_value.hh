// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_TRAIT_CH_FUNCTION_VALUE_HH
# define MLN_TRAIT_CH_FUNCTION_VALUE_HH

/// \file
///
/// Definition of the "change value" function trait.

# define mln_ch_function_value(I, V) \
typename mln::trait::ch_function_value< I, V >::ret

# define mln_ch_function_value_(I, V) \
mln::trait::ch_functionvalue< I, V >::ret

# include <mln/fun/v2v/ch_function_value.hh>

namespace mln
{

  // Forward declarations.
  namespace trait { template <typename F, typename V> struct ch_function_value; }
  namespace fun { namespace i2v { template <typename T> class array; } }


  namespace trait
  {

    namespace impl
    {

      // Default.
      template <typename F, typename VF, typename V>
      struct ch_function_value
      {
	typedef fun::v2v::ch_function_value<F, V> ret;
      };

      template <typename VF, typename V>
      struct ch_function_value<fun::i2v::array<VF>, VF, V>
      {
	typedef fun::i2v::array<V> ret;
      };

    } // end of namespace mln::trait::impl


    template <typename F, typename V>
    struct ch_function_value
    {
      typedef typename impl::ch_function_value<F, mln_result(F), V>::ret ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_CH_FUNCTION_VALUE_HH
