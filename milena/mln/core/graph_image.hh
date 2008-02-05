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

#ifndef MLN_CORE_GRAPH_IMAGE_HH
# define MLN_CORE_GRAPH_IMAGE_HH

/*! \file mln/core/graph_image.hh
 *
 *  \brief Definition of a graph-based image.
 */

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/metal/vec.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename V> struct graph_image;

  namespace internal
  {

    /// \internal Data structure for \c mln::graph_image<P,V>.
    template <typename P, typename V>
    struct data_< graph_image<P, V> >
    {
      data_(p_graph<P>& g, std::vector<V>& val);

      std::vector<V> val_;
      p_graph<P>& pg_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename V>
    struct image_< graph_image<P, V> > : default_image_< V, graph_image<P, V> >
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
  struct graph_image :
    public internal::image_primary_< p_graph<P>, graph_image<P, V> >
  {

    typedef mln::internal::image_base_< p_graph<P>, graph_image<P, V> > super_;

    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    typedef V& lvalue;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;


    /// Skeleton.
    typedef graph_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    graph_image(p_graph<P>& g, std::vector<V>& val);
    graph_image();

    /// Initialize an empty image.
    void init_(p_graph<P>& g, std::vector<V>& val);

    /// Read-only access of pixel value at point site \p p.
    const V& operator()(const graph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    V& operator()(const graph_psite<P>& p);

    /// Give the set of values of the image.
    const vset& values() const;

    // FIXME: Keep this name?
    const std::vector<V>& data_values () const;

    const p_graph<P>& domain() const;

    /// Return the first node of the link at i from loc
    const P& access_location_link_node1 (const unsigned& i) const;

    /// Return the second node of the link at i from loc
    const P& access_location_link_node2 (const unsigned& i) const;
};

  // Fwd decl.
  template <typename P, typename V>
  void init_(tag::image_t,
	     graph_image<P, V>& target, const graph_image<P, V>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <typename P, typename V>
  inline
  void init_(tag::image_t,
	     graph_image<P, V>& target, const graph_image<P, V>& model)
  {
    /* FIXME: Unfortunately, we cannot simply use 

         target.init_(model.domain(), model.data_values ());

       here, since domain() and data_values() return const data, and
       init_ expects non mutable data.  These constness problems exist
       also in graph_psite (see uses of const_cast<>).  Hence the
       inelegant use of const_cast<>'s.  */
    target.init_(const_cast<p_graph<P>&> (model.domain()),
		 const_cast<std::vector<V>&> (model.data_values ()));
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data_< graph_image<P, V> >::data_(p_graph<P>& g, std::vector<V>& val)
      : val_ (val),
	pg_ (g)
    {
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <typename P, typename V>
  inline
  graph_image<P, V>::graph_image(p_graph<P>& g, std::vector<V>& val)
  {
    init_(g, val);
  }

  template <typename P, typename V>
  inline
  graph_image<P, V>::graph_image()
  {
  }

  template <typename P, typename V>
  inline
  void
  graph_image<P, V>::init_(p_graph<P>& g, std::vector<V>& val)
  {
    /* FIXME: We leak memory here: calling init_ twice loses the
       previous content pointed by data_.

       We should definitely write down formal guidelines on
       initialization and memory management in general!  */
    this->data_ = new internal::data_< graph_image<P, V> > (g, val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  const V&
  graph_image<P, V>::operator()(const graph_psite<P>& p) const
  {
    mln_precondition(&p.pg_ == &this->data_->pg_);
    mln_precondition(p.i_ < this->data_->val_.size());
    return this->data_->val_[p.i_];
  }

  template <typename P, typename V>
  inline
  V&
  graph_image<P, V>::operator()(const graph_psite<P>& p)
  {
    mln_precondition(&p.pg_ == &this->data_->pg_);
    mln_precondition(p.i_ < this->data_->val_.size());
    return this->data_->val_[p.i_];
  }

  template <typename P, typename V>
  inline
  const mln::value::set<V> &
  graph_image<P, V>::values() const
  {
    return vset::the();
  }

  template <typename P, typename V>
  inline
  const std::vector<V>&
  graph_image<P, V>::data_values () const
  {
    return this->data_->val_;
  }

  template <typename P, typename V>
  inline
  const p_graph<P>&
  graph_image<P, V>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->pg_;
  }

  template <typename P, typename V>
  inline
  const P&
  graph_image<P, V>::access_location_link_node1 (const unsigned& i) const
  {
    // FIXME: This is ugly!  Too much implementation details are shown here.
    return this->domain().loc_[this->domain().gr_.links_[i]->pair_node_.first];
  }

  template <typename P, typename V>
  inline
  const P&
  graph_image<P, V>::access_location_link_node2 (const unsigned& i) const
  {
    // FIXME: This is ugly!  Too much implementation details are shown here.
    return this->domain().loc_[this->domain().gr_.links_[i]->pair_node_.second];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_GRAPH_IMAGE_HH
