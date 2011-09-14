// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_LINE_GRAPH_IMAGE_HH
# define MLN_CORE_IMAGE_LINE_GRAPH_IMAGE_HH

/// \file mln/core/image/line_graph_image.hh
/// \brief Definition of a line graph-based image.

# include <vector>

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/algebra/vec.hh>
# include <mln/core/site_set/p_line_graph.hh>
# include <mln/core/image/line_graph_psite.hh>
# include <mln/value/set.hh>

/* FIXME: This class shares a lot with graph_image.  Factor as much as
   possible.  */

/* FIXME: This is only a very naive prototype.  For instance, this
   image associates values to both the vertices and the edges of the
   graph, but only values on edges are accessible.  We probably want
   to fork this class to have a pure image of line graph (with no data
   on vertices) and one having data on both vertices and edges.

   Moreover, in the current implementation, the type of values on
   vertices and edges is necessarily the same (V).  We should allow
   different data types for vertices and edges.  */


namespace mln
{

  // Forward declaration.
  template <typename P, typename V> struct line_graph_image;

  namespace internal
  {

    /// Data structure for \c mln::line_graph_image<P,V>.
    template <typename P, typename V>
    struct data< line_graph_image<P, V> >
    {
      data(const p_line_graph<P>& plg,
	    const std::vector<V>& vertex_val, const std::vector<V>& edge_val);

      std::vector<V> vertex_val_;
      std::vector<V> edge_val_;
      const p_line_graph<P> plg_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename V>
    struct image_< line_graph_image<P, V> >
      : default_image_< V, line_graph_image<P, V> >
    {
      // Misc.
      typedef trait::image::category::primary category;
      // FIXME: Is that right?
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // Value.
      typedef trait::image::value_access::direct           value_access;
      // FIXME: Is that right?
      typedef trait::image::value_storage::one_block       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_io::read_write           value_io;

      // Site / domain.
      // FIXME: Depends on P.
      typedef trait::image::localization::basic_grid          localization;
      // FIXME: Likewise.
      typedef typename trait::image::space_from_point<P>::ret dimension;

      // Extended domain
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait


  /// \brief Image based on a graph.
  ///
  /// Values are stored on the edges of the graph.
  template <typename P, typename V>
  class line_graph_image
    : public internal::image_primary< V, p_line_graph<P>,
				      line_graph_image<P, V> >
  {
  public:
    /// Value associated type.
    typedef V value;

    /// \brief Return type of read-write access.
    ///
    /// We use the associated type \c reference instead of a plain
    /// reference on the value type (\p V), because it's the only way
    /// to safely form a reference on the element in the case of a
    /// std::vector<bool>.
    typedef typename std::vector<V>::reference lvalue;

    /// Return type of read-only access.
    typedef typename std::vector<V>::const_reference rvalue;

    /// Skeleton.
    typedef line_graph_image< tag::psite_<P>, tag::value_<V> > skeleton;

  public:
    /// Constructors.
    /// \{
    line_graph_image();
    line_graph_image(const p_line_graph<P>& plg);
    line_graph_image(const p_line_graph<P>& plg,
		     const std::vector<V>& vertex_val,
		     const std::vector<V>& edge_val);
    /// \}

    /// Initialize an empty image.
    void init_(const p_line_graph<P>& plg,
	       const std::vector<V>& vertex_val,
	       const std::vector<V>& edge_val);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const line_graph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const line_graph_psite<P>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_line_graph<P>& domain() const;

    /// Return the array of values associated to the edges.
    const std::vector<V>& edge_values() const;
    /// Return the array of values associated to the vertices.
    const std::vector<V>& vertex_values() const;
    /// \}

    /* FIXME: Do we want to provide these two methods? (at least, in
       the interface of the class?  */

    /// Return the point of the first vertex adjacent to the edge with
    /// id \a e.
    const P& vertex1(const util::edge_id& e) const;
    /// Return the point of the second vertex adjacent to the edge with
    /// id \a e.
    const P& vertex2(const util::edge_id& e) const;
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
		 std::vector<V>(model.vertex_values().size()),
		 std::vector<V>(model.edge_values().size()));
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data< line_graph_image<P, V> >::data(const p_line_graph<P>& plg,
					   const std::vector<V>& vertex_val,
					   const std::vector<V>& edge_val)
      : vertex_val_(vertex_val),
	edge_val_(edge_val),
	plg_(plg)
    {
      mln_precondition(plg.nvertices() == vertex_val.size());
      mln_precondition(plg.nedges() == edge_val.size());
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
  line_graph_image<P, V>::line_graph_image(const p_line_graph<P>& plg)
  {
    init_(plg, std::vector<V>(plg.nvertices()), std::vector<V>(plg.nedges()));
  }

  template <typename P, typename V>
  inline
  line_graph_image<P, V>::line_graph_image(const p_line_graph<P>& plg,
					   const std::vector<V>& vertex_val,
					   const std::vector<V>& edge_val)
  {
    init_(plg, vertex_val, edge_val);
  }

  template <typename P, typename V>
  inline
  void
  line_graph_image<P, V>::init_(const p_line_graph<P>& plg,
				const std::vector<V>& vertex_val,
				const std::vector<V>& edge_val)
  {
    mln_precondition(! this->is_valid());
    this->data_ =
      new internal::data< line_graph_image<P, V> >(plg, vertex_val, edge_val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  typename line_graph_image<P, V>::rvalue
  line_graph_image<P, V>::operator()(const line_graph_psite<P>& p) const
  {
    mln_precondition(p.is_valid());
    mln_precondition(p.site_set() == this->data_->plg_);
    return this->data_->edge_val_[p.edge_id()];
  }

  template <typename P, typename V>
  inline
  typename line_graph_image<P, V>::lvalue
  line_graph_image<P, V>::operator()(const line_graph_psite<P>& p)
  {
    mln_precondition(p.is_valid());
    mln_precondition(p.site_set() == this->data_->plg_);
    return this->data_->edge_val_[p.edge_id()];
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
  line_graph_image<P, V>::vertex_values() const
  {
    return this->data_->vertex_val_;
  }

  template <typename P, typename V>
  inline
  const p_line_graph<P>&
  line_graph_image<P, V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->plg_;
  }

  template <typename P, typename V>
  inline
  const P&
  line_graph_image<P, V>::vertex1(const util::edge_id& e) const
  {
    // FIXME: Improve the interface of graph to avoid these low-level
    // manipulations.
    util::vertex_id v1 = this->domain().gr_.edge(e).v1();
    return this->domain().gr_.vertex_data(v1);
  }

  template <typename P, typename V>
  inline
  const P&
  line_graph_image<P, V>::vertex2(const util::edge_id& e) const
  {
    // FIXME: Improve the interface of graph to avoid these low-level
    // manipulations.
    util::vertex_id v2 = this->domain().gr_.edge(e).v2();
    return this->domain().gr_.vertex_data(v2);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_LINE_GRAPH_IMAGE_HH
