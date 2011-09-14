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

#ifndef MLN_CORE_IMAGE_GRAPH_IMAGE_HH
# define MLN_CORE_IMAGE_GRAPH_IMAGE_HH

/// \file mln/core/image/graph_image.hh
/// \brief Definition of a graph-based image.

# include <vector>

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/algebra/vec.hh>
# include <mln/core/site_set/p_graph.hh>
# include <mln/core/image/bgraph_psite.hh>
# include <mln/value/set.hh>

/* FIXME: This class shares a lot with line_graph_image.  Factor as
   much as possible.  */


namespace mln
{

  // Fwd decl.
  template <typename P, typename V> struct graph_image;

  namespace internal
  {

    /// Data structure for \c mln::graph_image<P,V>.
    template <typename P, typename V>
    struct data< graph_image<P, V> >
    {
      data(const p_graph<P>& pg, const std::vector<V>& val);

      std::vector<V> val_;
      const p_graph<P> pg_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename V>
    struct image_< graph_image<P, V> > : default_image_< V, graph_image<P, V> >
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
  /// Values are stored on the edges of the graph, not on its vertices.
  template <typename P, typename V>
  class graph_image :
    public internal::image_primary< V, p_graph<P>, graph_image<P, V> >
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
    typedef graph_image< tag::psite_<P>, tag::value_<V> > skeleton;

  public:
    /// Constructors.
    /// \{
    graph_image();
    graph_image(const p_graph<P>& pg);
    graph_image(const p_graph<P>& pg, const std::vector<V>& val);
    /// \}

    /// Initialize an empty image.
    void init_(const p_graph<P>& pg, const std::vector<V>& val);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const graph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const graph_psite<P>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_graph<P>& domain() const;

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
	     graph_image<P, V>& target, const graph_image<P, W>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <typename P, typename V, typename W>
  inline
  void init_(tag::image_t,
	     graph_image<P, V>& target, const graph_image<P, W>& model)
  {
    target.init_(model.domain(),
		 std::vector<V>(model.vertex_values().size()));
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data< graph_image<P, V> >::data(const p_graph<P>& pg,
				      const std::vector<V>& val)
      : val_ (val),
	pg_ (pg)
    {
      mln_precondition(pg.nvertices() == val.size());
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <typename P, typename V>
  inline
  graph_image<P, V>::graph_image()
  {
  }

  template <typename P, typename V>
  inline
  graph_image<P, V>::graph_image(const p_graph<P>& pg)
  {
    init_(pg, std::vector<V>(pg.nvertices()));
  }

  template <typename P, typename V>
  inline
  graph_image<P, V>::graph_image(const p_graph<P>& pg,
				 const std::vector<V>& val)
  {
    init_(pg, val);
  }

  template <typename P, typename V>
  inline
  void
  graph_image<P, V>::init_(const p_graph<P>& pg, const std::vector<V>& val)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< graph_image<P, V> > (pg, val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  typename graph_image<P, V>::rvalue
  graph_image<P, V>::operator()(const graph_psite<P>& p) const
  {
    mln_precondition(p.is_valid());
    mln_precondition(p.site_set() == this->data_->pg_);
    return this->data_->val_[p.vertex_id()];
  }

  template <typename P, typename V>
  inline
  typename graph_image<P, V>::lvalue
  graph_image<P, V>::operator()(const graph_psite<P>& p)
  {
    mln_precondition(p.is_valid());
    mln_precondition(p.site_set() == this->data_->pg_);
    return this->data_->val_[p.vertex_id()];
  }

  template <typename P, typename V>
  inline
  const std::vector<V>&
  graph_image<P, V>::vertex_values() const
  {
    return this->data_->val_;
  }

  template <typename P, typename V>
  inline
  const p_graph<P>&
  graph_image<P, V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->pg_;
  }

  template <typename P, typename V>
  inline
  const P&
  graph_image<P, V>::vertex1(const util::edge_id& e) const
  {
    return this->domain().vertex1(e);
  }

  template <typename P, typename V>
  inline
  const P&
  graph_image<P, V>::vertex2(const util::edge_id& e) const
  {
    return this->domain().vertex2(e);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_IMAGE_HH
