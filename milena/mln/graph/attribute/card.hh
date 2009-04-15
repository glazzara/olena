// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef MLN_GRAPH_ATTRIBUTE_CARD_HH
# define MLN_GRAPH_ATTRIBUTE_CARD_HH

/// \file mln/graph/attribute/card.hh
/// Functor that computes the cardinality of every component in a graph.
///
/// \sa graph::compute


# include <mln/core/concept/graph.hh>
# include <mln/util/array.hh>


namespace mln
{

  namespace graph
  {

    namespace attribute
    {

      /// Functor that computes the cardinality of every component in a graph.
      struct card_t;

      /// This is a global instance of this functor.
      extern card_t card;


# ifndef MLN_INCLUDE_ONLY

      /// Compute the cardinality of every component in a graph.
      ///
      /// \return An array with the cardinality for each component.
      /// Components are labeled from 0.
      struct card_t
      {
	/// Type of the computed value
	typedef util::array<unsigned> result;

	template <typename G>
	void init(const Graph<G>& g)
	{
	  data.resize(0);
	  deja_vu_.resize(exact(g).v_nmax());
	  deja_vu_.fill(false);
	  comp = 0;
	}

	bool to_be_treated(unsigned id)
	{ return !deja_vu_[id]; }

	void new_component_from_vertex(unsigned id)
	{
	  data.append(1);
	  deja_vu_[id] = true;
	}

	void process_vertex(unsigned)
	{}

	bool to_be_queued(unsigned id)
	{ return !deja_vu_[id]; }

	void added_to_queue(unsigned id)
	{
	  deja_vu_[id] = true;
	  ++data[comp];
	}

	void next_component()
	{ ++comp; }

	void final()
	{}

	unsigned comp;
	util::array<bool> deja_vu_;
	util::array<unsigned> data;
      };

      card_t card;

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::graph::attribute

  } // end of namespace mln::graph

} // end of namespace mln


#endif // ! MLN_GRAPH_ATTRIBUTE_CARD_HH
