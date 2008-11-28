// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_CONCEPT_GRAPH_HH
# define MLN_CORE_CONCEPT_GRAPH_HH

/// \file mln/core/concept/graph.hh
///
/// Definition of the concept of mln::Graph.

# include <mln/core/concept/object.hh>

namespace mln
{


  // Forward declaration.
  template <typename E> struct Graph;

  // Graph category flag type.
  template <>
  struct Graph<void>
  {
    typedef Object<void> super;
  };

  /// Base class for implementation of graph classes.
  ///
  /// \see mln::doc::Graph for a complete documentation of this class
  /// contents.
  ///
  template <typename E>
  struct Graph : public Object<E>
  {
    typedef Graph<void> category;

    /*
      // provided by internal::image_base:

      typedef pset;
      typedef site;
      typedef psite;

      typedef fwd_piter;
      typedef bkd_piter;

      // Misc.
      const E& id() const;
      template<typename G2>
      bool is_subgraph_of(const G2& gr) const;

      // Vertex and edges oriented.
      unsigned v_other(unsigned id_e, unsigned id_v) const;

      // Vertex oriented.
      size_t v_nmax() const;
      bool has_v(unsigned id_v) const;
      size_t v_nmax_nbh_edges(unsigned id_v) const;
      unsigned v_ith_nbh_edge(unsigned id_v, unsigned i) const;

      // Edge oriented.
      size_t e_nmax() const;
      bool has_e(unsigned id_e) const;
      unsigned v1(unsigned id_e) const;
      unsigned v2(unsigned id_e) const;
      size_t e_nmax_nbh_edges(unsigned id_e) const;
      unsigned e_ith_nbh_edge(unsigned id_e, unsigned i) const;

     */

  protected:
    Graph();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Graph<E>::Graph()
  {
    // provided by internal::graph_base:

    //typedef mln_psite(E) psite;

    //typedef mln_fwd_piter(E) fwd_piter;
    //typedef mln_bkd_piter(E) bkd_piter;

    // Check methods
    const void* (E::*m1)() const = & E::id;
    m1 = 0;
    unsigned (E::*m2)(unsigned id_e, unsigned id_v) const = & E::v_other;
    m2 = 0;
    size_t (E::*m4)() const = & E::v_nmax;
    m4 = 0;
    bool (E::*m5)(unsigned id_v) const = & E::has_v;
    m5 = 0;
    size_t (E::*m6)(unsigned id_v) const = & E::v_nmax_nbh_edges;
    m6 = 0;
    unsigned (E::*m7)(unsigned id_v, unsigned i) const = & E::v_ith_nbh_edge;
    m7 = 0;
    size_t (E::*m8)() const = & E::e_nmax;
    m8 = 0;
    bool (E::*m9)(unsigned id_e) const = & E::has_e;
    m9 = 0;
    unsigned (E::*m10)(unsigned id_e) const = & E::v1;
    m10 = 0;
    unsigned (E::*m11)(unsigned id_e) const = & E::v2;
    m11 = 0;
    size_t (E::*m12)(unsigned id_e) const = & E::e_nmax_nbh_edges;
    m12 = 0;
    unsigned (E::*m13)(unsigned id_e, unsigned i) const = & E::e_ith_nbh_edge;
    m13 = 0;

    //FIXME: enable this test. Currently does not work because this is
    // a templated method.
    //bool (E::*m14)(...) = & E::is_subgraph_of;
    //m14 = 0;
}

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_CONCEPT_GRAPH_HH

