// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_LINE_GRAPH_IMAGE_HH
# define MLN_CORE_LINE_GRAPH_IMAGE_HH

/// \file mln/core/line_graph_image.hh
/// \brief Definition of a line graph-based image.

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/metal/vec.hh>
# include <mln/core/p_line_graph.hh>
# include <mln/core/line_graph_psite.hh>
# include <mln/value/set.hh>
# include <vector>

/* FIXME: This class shares a lot with graph_image.  Factor as much as
   possible.  */

/* FIXME: This is only a very naive prototype.  For instance, this
   image associates values to both the nodes and the edges of the
   graph, but only values on edges are accessible.  We probably want
   to fork this class to have a pure image of line graph (with no data
   on nodes) and one having data on both nodes and edges.

   Moreover, in the current implementation, the type of values on
   nodes and edges is necessarily the same (V).  We should allow
   different data types for nodes and edges.  */


namespace mln
{

  // Fwd decl.
  template <typename P, typename V> struct line_graph_image;

  namespace internal
  {

    /// \internal Data structure for \c mln::line_graph_image<P,V>.
    template <typename P, typename V>
    struct data_< line_graph_image<P, V> >
    {
      data_(const p_line_graph<P>& g,
	    const std::vector<V>& node_val, const std::vector<V>& edge_val);

      std::vector<V> node_val_;
      std::vector<V> edge_val_;
      /* The line graph point set is handled by address, so that we
         can check the compatibility of images w.r.t. to their point
         sites.  We could use a safer (and more complex) facility to
         ensure (memory) equality of line graph point sets, but using
         addresses is simple and efficient enough for the moment.  */
      const p_line_graph<P>& plg_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename V>
    struct image_< line_graph_image<P, V> >
      : default_image_< V, line_graph_image<P, V> >
    {
      typedef trait::image::category::primary category;

      // FIXME: Is that right?
      typedef trait::image::access::random                    access;
      typedef typename trait::image::space_from_point<P>::ret space;
      typedef trait::image::size::regular                     size;
      typedef trait::image::support::irregular                support;

      typedef trait::image::border::none                      border;
      typedef trait::image::data::stored                      data;
      typedef trait::image::io::read_write                    io;
      // FIXME: Is that right?
      typedef trait::image::speed::fast                       speed;
    };

  } // end of namespace mln::trait


  /// \brief Image based on a graph.
  ///
  /// Values are stored on the vertices of the graph.
  template <typename P, typename V>
  struct line_graph_image :
    public internal::image_primary_< p_line_graph<P>, line_graph_image<P, V> >
  {
    /// Super type.
    typedef mln::internal::image_base_< p_line_graph<P>,
					line_graph_image<P, V> > super_;

    /// Value associated type.
    typedef V value;

    /// \brief Return type of read-write access.
    ///
    /// We use the associated type \c reference instead of a plain
    /// reference on th value type (\v V), because it's the only way
    /// to safely form a reference on the element in the case of a
    /// std::vector<bool>.
    typedef typename std::vector<V>::reference lvalue;

    /// Return type of read-only access.
    typedef typename std::vector<V>::const_reference rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;

    /// Skeleton.
    typedef line_graph_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    /// \{
    line_graph_image();
    line_graph_image(const p_line_graph<P>& g);
    line_graph_image(const p_line_graph<P>& g,
		     const std::vector<V>& node_val,
		     const std::vector<V>& edge_val);
    /// \}

    /// Initialize an empty image.
    void init_(const p_line_graph<P>& g,
	       const std::vector<V>& node_val,
	       const std::vector<V>& edge_val);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const line_graph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const line_graph_psite<P>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_line_graph<P>& domain() const;
    /// Return the domain of values of the image.
    const vset& values() const;

    /// Return the array of values associated to the edges.
    const std::vector<V>& edge_values() const;
    /// Return the array of values associated to the nodes.
    const std::vector<V>& node_values() const;
    /// \}

    /* FIXME: Do we want to provide these two methods? (at least, in
       the interface of the class?  */

    /// Return the point of the first node adjacent to the edge with
    /// id \a e.
    const P& node1(const util::edge_id& e) const;
    /// Return the point of the second node adjacent to the edge with
    /// id \a e.
    const P& node2(const util::edge_id& e) const;
};

  // Fwd decl.
  template <typename P, typename V, typename W>
  void init_(tag::image_t,
	     line_graph_image<P, V>& target,
	     const line_graph_image<P, W>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <typename P, typename V, typename W>
  inline
  void init_(tag::image_t,
	     line_graph_image<P, V>& target,
	     const line_graph_image<P, W>& model)
  {
    target.init_(model.domain(),
		 std::vector<V>(model.node_values().size()),
		 std::vector<V>(model.edge_values().size()));
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data_< line_graph_image<P, V> >::data_(const p_line_graph<P>& g,
					   const std::vector<V>& node_val,
					   const std::vector<V>& edge_val)
      : node_val_(node_val),
	edge_val_(edge_val),
	plg_(g)
    {
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <typename P, typename V>
  inline
  line_graph_image<P, V>::line_graph_image()
  {
  }

  template <typename P, typename V>
  inline
  line_graph_image<P, V>::line_graph_image(const p_line_graph<P>& g)
  {
    init_(g, std::vector<V>(g.nnodes()), std::vector<V>(g.nedges()));
  }

  template <typename P, typename V>
  inline
  line_graph_image<P, V>::line_graph_image(const p_line_graph<P>& g,
					   const std::vector<V>& node_val,
					   const std::vector<V>& edge_val) 
  {
    init_(g, node_val, edge_val);
  }

  template <typename P, typename V>
  inline
  void
  line_graph_image<P, V>::init_(const p_line_graph<P>& g,
				const std::vector<V>& node_val,
				const std::vector<V>& edge_val)
  {
    mln_precondition(! this->has_data());
    this->data_ =
      new internal::data_< line_graph_image<P, V> >(g, node_val, edge_val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  typename line_graph_image<P, V>::rvalue
  line_graph_image<P, V>::operator()(const line_graph_psite<P>& p) const
  {
    mln_precondition(&p.plg() == &this->data_->plg_);
    mln_precondition(p.id() < this->data_->edge_val_.size());
    return this->data_->edge_val_[p.id()];
  }

  template <typename P, typename V>
  inline
  typename line_graph_image<P, V>::lvalue
  line_graph_image<P, V>::operator()(const line_graph_psite<P>& p)
  {
    mln_precondition(&p.plg() == &this->data_->plg_);
    mln_precondition(p.id() < this->data_->edge_val_.size());
    return this->data_->edge_val_[p.id()];
  }

  template <typename P, typename V>
  inline
  const mln::value::set<V> &
  line_graph_image<P, V>::values() const
  {
    return vset::the();
  }

  template <typename P, typename V>
  inline
  const std::vector<V>&
  line_graph_image<P, V>::edge_values() const
  {
    return this->data_->edge_val_;
  }

  template <typename P, typename V>
  inline
  const std::vector<V>&
  line_graph_image<P, V>::node_values() const
  {
    return this->data_->node_val_;
  }

  template <typename P, typename V>
  inline
  const p_line_graph<P>&
  line_graph_image<P, V>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->plg_;
  }

  template <typename P, typename V>
  inline
  const P&
  line_graph_image<P, V>::node1(const util::edge_id& e) const
  {
    // FIXME: Improve the interface of graph to avoid these low-level
    // manipulations.
    util::node_id n1 = this->domain().gr_.edge(e).n1();
    return this->domain().gr_.node_data(n1);
  }

  template <typename P, typename V>
  inline
  const P&
  line_graph_image<P, V>::node2(const util::edge_id& e) const
  {
    // FIXME: Improve the interface of graph to avoid these low-level
    // manipulations.
    util::node_id n2 = this->domain().gr_.edge(e).n2();
    return this->domain().gr_.node_data(n2);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LINE_GRAPH_IMAGE_HH
