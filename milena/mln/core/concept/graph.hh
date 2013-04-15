// Copyright (C) 2008, 2009, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_GRAPH_HH
# define MLN_CORE_CONCEPT_GRAPH_HH

/// \file
///
/// Definition of the concept of mln::Graph.

# include <mln/core/concept/object.hh>
# include <mln/util/graph_ids.hh>

namespace mln
{


  // Forward declaration.
  template <typename E> struct Graph;

  /// \cond INTERNAL_API
  /// Graph category flag type.
  template <>
  struct Graph<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation of graph classes.

    \ingroup modconcepts
  */
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
    */
    /*
      // Vertex and edges oriented.
      util::vertex_id_t v_other(const util::edge_id_t& id_e, const util::vertex_id_t& id_v) const;

      // Vertex oriented.
      size_t v_nmax() const;
      bool has(unsigned id_v) const;
      size_t v_nmax_nbh_edges(const util::vertex_id_t& id_v) const;
      util::edge_id_t v_ith_nbh_edge(const util::vertex_id_t& id_v, unsigned i) const;

      // Edge oriented.
      size_t e_nmax() const;
      bool has_e(const util::edge_id_t& id_e) const;
      util::vertex_id_t v1(const util::edge_id_t& id_e) const;
      util::vertex_id_t v2(const util::edge_id_t& id_e) const;
      size_t e_nmax_nbh_edges(const util::edge_id_t& id_e) const;
      util::edge_id_t e_ith_nbh_edge(const util::edge_id_t& id_e, unsigned i) const;

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
    (void) m1;
    util::vertex_id_t (E::*m2)(const util::edge_id_t& id_e, const util::vertex_id_t& id_v) const = & E::v_other;
    (void) m2;
    size_t (E::*m4)() const = & E::v_nmax;
    (void) m4;
    bool (E::*m5)(const util::vertex_id_t& id_v) const = & E::has_v;
    (void) m5;
    size_t (E::*m6)(const util::vertex_id_t& id_v) const = & E::v_nmax_nbh_edges;
    (void) m6;
    util::edge_id_t (E::*m7)(const util::vertex_id_t& id_v, unsigned i) const = & E::v_ith_nbh_edge;
    (void) m7;
    size_t (E::*m8)() const = & E::e_nmax;
    (void) m8;
    bool (E::*m9)(const util::edge_id_t& id_e) const = & E::has_e;
    (void) m9;
    util::vertex_id_t (E::*m10)(const util::edge_id_t& id_e) const = & E::v1;
    (void) m10;
    util::vertex_id_t (E::*m11)(const util::edge_id_t& id_e) const = & E::v2;
    (void) m11;
    size_t (E::*m12)(const util::edge_id_t& id_e) const = & E::e_nmax_nbh_edges;
    (void) m12;
    util::edge_id_t (E::*m13)(const util::edge_id_t& id_e, unsigned i) const = & E::e_ith_nbh_edge;
    (void) m13;

    bool (E::*m14)() const = & E::is_valid;
    (void) m14;
    void (E::*m15)() = & E::invalidate;
    (void) m15;
    //FIXME: enable this test. Currently does not work because this is
    // a templated method.
    //bool (E::*m14)(...) = & E::is_subgraph_of;
    //(void) m14;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_CONCEPT_GRAPH_HH
