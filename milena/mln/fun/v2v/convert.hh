// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_V2V_CONVERT_HH
# define MLN_FUN_V2V_CONVERT_HH

/// \file
///
/// FIXME.
///
/// \todo The function is intrisically meta; how to handle that
/// particular case?

# include <mln/core/concept/function.hh>
# include <mln/convert/to.hh>



namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      // FIXME: Doc!

      template <typename V>
      struct convert : public Function_v2v< convert<V> >
      {
	typedef V result;

	template <typename W>
	V operator()(const W& w) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      template <typename W>
      inline
      V
      convert<V>::operator()(const W& w) const
      {
	return mln::convert::to<V>(w);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_CONVERT_HH
