// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_VV2B_EQ_HH
# define MLN_FUN_VV2B_EQ_HH

/// \file
///
/// Functor that computes equal between two values.

# include <mln/core/concept/function.hh>
# include <mln/trait/op/and.hh>


namespace mln
{

  namespace fun
  {

    namespace vv2b
    {

      /// Functor computing equal between two values.
      template <typename L, typename R = L>
      struct eq : public Function_vv2b< eq<L,R> >
      {
	bool operator()(const L& v1, const R& v2) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename L, typename R>
      inline
      bool
      eq<L,R>::operator()(const L& v1, const R& v2) const
      {
	return v1 == v2;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::vv2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_VV2B_EQ_HH
