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
      data_(p_line_graph<P>& g,
	    std::vector<V>& node_val, std::vector<V>& edge_val);

      std::vector<V> node_val_;
      std::vector<V> edge_val_;
      p_line_graph<P>& plg_;
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

  /*! \brief Kind of image based on graph structure.
   *
   */
  template <typename P, typename V>
  struct line_graph_image :
    public internal::image_primary_< p_line_graph<P>, line_graph_image<P, V> >
  {

    typedef mln::internal::image_base_< p_line_graph<P>,
					line_graph_image<P, V> > super_;

    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    typedef V& lvalue;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;


    /// Skeleton.
    typedef line_graph_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    line_graph_image(p_line_graph<P>& g,
		     std::vector<V>& node_val, std::vector<V>& edge_val);
    line_graph_image();

    /// Initialize an empty image.
    void init_(p_line_graph<P>& g,
	       std::vector<V>& node_val, std::vector<V>& edge_val);

    /// Read-only access of pixel value at point site \p p.
    const V& operator()(const line_graph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    V& operator()(const line_graph_psite<P>& p);

    /// Give the set of values of the image.
    const vset& values() const;

    // FIXME: Keep this name?
    const std::vector<V>& data_values() const;

    const p_line_graph<P>& domain() const;

    // FIXME: Do we want to provide these two methods?

    /// Return the point of the first node adjacent to the edge with
    /// id \a e.
    const P& node1(const util::edge_id& e) const;
    /// Return the point of the second node adjacent to the edge with
    /// id \a e.
    const P& node2(const util::edge_id& e) const;
};

  // Fwd decl.
  template <typename P, typename V>
  void init_(tag::image_t,
	     line_graph_image<P, V>& target,
	     const line_graph_image<P, V>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <typename P, typename V>
  inline
  void init_(tag::image_t,
	     line_graph_image<P, V>& target,
	     const line_graph_image<P, V>& model)
  {
    /* FIXME: Unfortunately, we cannot simply use 

         target.init_(model.domain(), model.data_values());

       here, since domain() and data_values() return const data, and
       init_ expects non mutable data.  These constness problems exist
       also in line_graph_psite (see uses of const_cast<>).  Hence the
       inelegant use of const_cast<>'s.  */
    target.init_(const_cast<p_line_graph<P>&>(model.domain()),
		 const_cast<std::vector<V>&>(model.data_values()));
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data_< line_graph_image<P, V> >::data_(p_line_graph<P>& g,
					   std::vector<V>& node_val,
					   std::vector<V>& edge_val)
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
  line_graph_image<P, V>::line_graph_image(p_line_graph<P>& g,
					   std::vector<V>& node_val,
					   std::vector<V>& edge_val) 
 {
   init_(g, node_val, edge_val);
  }

  template <typename P, typename V>
  inline
  line_graph_image<P, V>::line_graph_image()
  {
  }

  template <typename P, typename V>
  inline
  void
  line_graph_image<P, V>::init_(p_line_graph<P>& g,
				std::vector<V>& node_val,
				std::vector<V>& edge_val)
  {
    /* FIXME: We leak memory here: calling init_ twice loses the
       previous content pointed by data_.

       We should definitely write down formal guidelines on
       initialization and memory management in general!  */
    this->data_ =
      new internal::data_< line_graph_image<P, V> >(g, node_val, edge_val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  const V&
  line_graph_image<P, V>::operator()(const line_graph_psite<P>& p) const
  {
    mln_precondition(&p.plg() == &this->data_->plg_);
    mln_precondition(p.id() < this->data_->edge_val_.size());
    return this->data_->edge_val_[p.id()];
  }

  template <typename P, typename V>
  inline
  V&
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
  line_graph_image<P, V>::data_values() const
  {
    return this->data_->edge_val_;
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
