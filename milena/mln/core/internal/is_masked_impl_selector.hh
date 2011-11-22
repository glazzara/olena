// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_INTERNAL_IS_MASKED_IMPL_SELECTOR_HH
# define MLN_CORE_INTERNAL_IS_MASKED_IMPL_SELECTOR_HH

/// \file
///
/// \brief Add a specific member to graph windows/neighborhoods.

# include <mln/util/edge.hh>


namespace mln
{

  // Forward declarations.
  template <typename G, typename F> class p_edges;
  template <typename G, typename F> class p_vertices;



  namespace internal
  {

    /// Default implementation. D == S.
    /// D and S are site sets.
    template <typename S, typename D, typename E>
    struct is_masked_impl_selector
    {
      /// Return wheter a graph element must be masked by this window
      bool is_masked(const mln_graph_element(S)& center,
		     const mln_graph_element(S)& element) const;
    };

    /// Restrict iteration on vertices according to masked edges.
    template <typename G1, typename F1, typename G2, typename F2, typename E>
    struct is_masked_impl_selector< p_vertices<G1,F1>, p_edges<G2,F2>, E >
    {
      typedef p_vertices<G1,F1> S;

      /// Return wheter a graph element must be masked by this window
      bool is_masked(const mln_graph_element(S)& center,
		     const mln_graph_element(S)& element) const;
    };

    /// Restrict iteration on edges according to masked vertices.
    template <typename G1, typename F1, typename G2, typename F2, typename E>
    struct is_masked_impl_selector< p_edges<G1,F1>, p_vertices<G2,F2>, E >
    {
      typedef p_edges<G1,F1> S;

      /// Return wheter a graph element must be masked by this window
      bool is_masked(const mln_graph_element(S)& center,
		     const mln_graph_element(S)& element) const;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename S, typename I, typename E>
    inline
    bool
    is_masked_impl_selector<S,I,E>::is_masked(const mln_graph_element(S)& center,
					      const mln_graph_element(S)& element) const
    {
      (void) center;
      E& iter = internal::force_exact<E>(*this);
      return ! iter.target_()->mask()(element.id());
    }


    template <typename G1, typename F1, typename G2, typename F2, typename E>
    inline
    bool
    is_masked_impl_selector< p_vertices<G1,F1>, p_edges<G2,F2>, E >
      ::is_masked(const mln_graph_element(S)& center,
		  const mln_graph_element(S)& element) const
    {
      E& iter = internal::force_exact<E>(*this);

      util::edge<G1> e = center.edge_with(element);
      mln_postcondition(e.is_valid());

      return ! iter.target_()->mask()(e.id());
    }


    ///    center       element
    /// X -------- X ------------ X
    ///
    /// We try to find the common vertex.
    /// Then we test if the other one is masked or not.
    ///
    template <typename G1, typename F1, typename G2, typename F2, typename E>
    inline
    bool
    is_masked_impl_selector< p_edges<G1,F1>, p_vertices<G2,F2>, E >
      ::is_masked(const mln_graph_element(S)& center,
		  const mln_graph_element(S)& element) const
    {
      E& iter = internal::force_exact<E>(*this);

      if (center.v1() == element.v2() || center.v2() == element.v2())
	return ! iter.target_()->mask()(element.v1());

      //else if (center.v2() == element.v1() || center.v1() == element.v1())
      return ! iter.target_()->mask()(element.v2());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IS_MASKED_IMPL_SELECTOR_HH
