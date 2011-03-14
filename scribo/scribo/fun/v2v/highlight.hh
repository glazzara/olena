// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_FUN_V2V_HIGHLIGHT_HH
# define SCRIBO_FUN_V2V_HIGHLIGHT_HH

/// \brief Function increasing values to highlight areas.

# include <mln/core/concept/function.hh>

namespace scribo
{

  namespace fun
  {

    namespace v2v
    {

	template <typename R>
	struct highlight
	  : mln::Function_v2v<highlight<R> >
	{
	  typedef R result;

	  highlight();
	  result operator()(const result& v) const;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename R>
	highlight<R>::highlight()
	{
	}


	template <typename R>
	typename highlight<R>::result
	highlight<R>::operator()(const result& v) const
	{
	  return v + v / 2;
	}

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::fun::v2v

  } // end of namespace scribo::fun

} // end of namespace scribo

#endif // ! SCRIBO_FUN_V2V_HIGHLIGHT_HH
