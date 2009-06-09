// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_INTERNAL_ID2ELEMENT_HH
# define MLN_UTIL_INTERNAL_ID2ELEMENT_HH

/// \file
///
/// Function constructing a element from an id.

#include <mln/core/concept/graph.hh>
#include <mln/core/concept/function.hh>

namespace mln
{

  namespace util
  {

    namespace internal
    {

      template <typename G, typename Elt>
      struct id2element : Function_v2v< id2element<G,Elt> >
      {
	typedef Elt result;

	id2element();
	id2element(const Graph<G>& g);
	Elt operator()(unsigned id) const;

	G g_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename G, typename Elt>
      inline
      id2element<G,Elt>::id2element()
      {
      }

      template <typename G, typename Elt>
      inline
      id2element<G,Elt>::id2element(const Graph<G>& g)
	: g_(exact(g))
      {
      }

      template <typename G, typename Elt>
      inline
      Elt
      id2element<G,Elt>::operator()(unsigned id) const
      {
	mln_assertion(g_.is_valid());
	mln_assertion(g_.has(Elt(g_,id)));
	/// FIXME: cannot check whether the graph has
	/// its element since we must call the proper member according
	/// to the element type (has_v or has_e)!
	return Elt(g_, id);
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::util::internal

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_INTERNAL_ID2ELEMENT_HH
