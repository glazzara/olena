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

#ifndef MLN_CORE_CONCEPT_TREE_HH
# define MLN_CORE_CONCEPT_TREE_HH

# include <mln/core/concept/object.hh>

namespace mln
{

  // Forward declaration.
  template <typename E> struct Tree;

  // Tree category flag type.
  template <>
  struct Tree<void>
  {
    typedef Object<void> super;
  };

  template <typename E>
  struct Tree : public Object<E>
  {
    typedef Tree<void> category;

    /*
    // Original image related
    typedef site;
    typedef psite;

    typedef value;
    typedef rvalue;
    typedef lvalue;

    // Node/Site related material.
    typedef domain_t;
    const domain_t& domain() const;

    typedef nodes_t;
    const nodes_t& nodes() const;

    bool has(const node_t&) const;
    bool has(const psite&) const;

    // Value related material.
    rvalue f() (const psite&) const;
    lvalue f() (const psite&);
    rvalue f[] (const node_t&) const;
    lvalue f[] (const node_t&);

    // Node relationship.
    node_t node(const psite&) const;
    void set_node(const psite&, const node_t&);
    node_t parent(const node_id_t&) const;
    void set_parent(const node_t&, const node_t&);

    // Node test materials
    bool is_a_leaf(const node_t&) const;
    bool is_a_root(const node_t&) const;
    */

  protected:
    Tree();
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Tree<E>::Tree()
  {
    typedef mln_site(E) site;
    typedef mln_psite(E) psite;

    typedef mln_value(E) value;
    typedef mln_rvalue(E) rvalue;
    typedef mln_lvalue(E) lvalue;

    typedef typename E::node_t node_t;

    // Site/Node related material.
    typedef mln_domain(E) domain_t;
    typedef typename E::nodes_t nodes_t;

    const domain_t& (E::*m1)() const = & E::domain;
    m1 = 0;
    const nodes_t& (E::*m2)() const = & E::nodes;
    m2 = 0;
    bool (E::*m3)(const node_t&) const = & E::has;
    m3 = 0;
    bool (E::*m4)(const psite&) const = & E::has;
    m4 = 0;

    // Value related material.
    rvalue (E::*m5)(const psite&) const = & E::f;
    m5 = 0;
    lvalue (E::*m6)(const psite&) = & E::f;
    m6 = 0;
    rvalue (E::*m7)(const node_t&) const = & E::f;
    m7 = 0;
    lvalue (E::*m8)(const node_t&) = & E::f;
    m8 = 0;

    // Node relationship.
    node_t (E::*m9)(const psite&) const = & E::node;
    m9 = 0;
    void (E::*m10)(const psite&, const node_t&) = & E::set_node;
    m10 = 0;
    node_t (E::*m11)(const node_t&) const = & E::parent;
    m11 = 0;
    void (E::*m12)(const node_t&, const node_t&) = & E::set_parent;
    m12 = 0;

    // Node tests
    bool (E::*m13)(const node_t&) const = & E::is_a_leaf;
    m13 = 0;
    bool (E::*m14)(const node_t&) const = & E::is_a_root;
    m14 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_CORE_CONCEPT_TREE_HH
