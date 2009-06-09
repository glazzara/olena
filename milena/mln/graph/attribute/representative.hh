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

#ifndef MLN_GRAPH_ATTRIBUTE_REPRESENTATIVE_HH
# define MLN_GRAPH_ATTRIBUTE_REPRESENTATIVE_HH

/// \file
///
/// Functor that computes the representative vertex of every component in
/// a graph.
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

      /// Functor that computes the representative vertex of every component
      /// in a graph.
      struct representative_t;

      /// This is a global instance of this functor.
      extern representative_t representative;

# ifndef MLN_INCLUDE_ONLY

      /// Compute the representative vertex of every component in a graph.
      ///
      /// \return An array with the representative for each component.
      /// Components are labeled from 0.
      struct representative_t
      {
	/// Type of the computed value
	typedef util::array<unsigned> result;

	template <typename G>
	void init(const Graph<G>& g)
	{
	  data.resize(0);
	  deja_vu_.resize(exact(g).v_nmax());
	  deja_vu_.fill(false);
	}

	bool to_be_treated(unsigned id)
	{ return !deja_vu_[id]; }

	void new_component_from_vertex(unsigned id)
	{
	  data.append(id);
	  deja_vu_[id] = true;
	}

	void process_vertex(unsigned)
	{}

	bool to_be_queued(unsigned id)
	{ return !deja_vu_[id]; }

	void added_to_queue(unsigned id)
	{
	  deja_vu_[id] = true;
	}

	void next_component()
	{}

	void final()
	{}

	util::array<bool> deja_vu_;
	util::array<unsigned> data;
      };

      representative_t representative;


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::graph::attribute

  } // end of namespace mln::graph

} // end of namespace mln


#endif // ! MLN_GRAPH_ATTRIBUTE_REPRESENTATIVE_HH
