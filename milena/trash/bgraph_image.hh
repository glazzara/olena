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

#ifndef MLN_CORE_IMAGE_BGRAPH_IMAGE_HH
# define MLN_CORE_IMAGE_BGRAPH_IMAGE_HH

/// \file mln/core/image/bgraph_image.hh
///
/// Definition of a boost-graph-based image.

# include <mln/trait/images.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/algebra/vec.hh>

# include <mln/core/site_set/p_bgraph.hh>
# include <mln/core/image/bgraph_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Forward declaration
  template <typename P, typename V> struct bgraph_image;

  namespace internal
  {

    /// Data structure for \c mln::bgraph_image<P,V>.
    template <typename P, typename V>
    struct data< bgraph_image<P, V> >
    {
      /// Data stores a **copy** of the pset g.
      /// But, the pset g and the data_ copy will shared the same
      /// underlaying graph.
      data(const p_bgraph<P>& g, const std::vector<V>& val);


      p_bgraph<P> pg_;

      std::vector<V> val_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename V>
    struct image_< bgraph_image<P, V> > :
      default_image_< V, bgraph_image<P, V> >
    {
      typedef trait::image::category::primary category;

      // FIXME: update with new properties.
//      typedef trait::image::access::random                    access;
//      typedef typename trait::image::space_from_point<P>::ret space;
//      typedef trait::image::size::regular                     size;
//      typedef trait::image::support::irregular                support;
//
//      typedef trait::image::border::none                      border;
//      typedef trait::image::data::stored                      data;
//      typedef trait::image::io::read_write                    io;
//      typedef trait::image::speed::fast                       speed;
    };

  } // end of namespace mln::trait

  /// Kind of image based on a boost graph structure.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename P, typename V>
  struct bgraph_image :
    public internal::image_primary< V, p_bgraph<P>, bgraph_image<P, V> >
  {

    typedef mln::internal::image_primary< V, p_bgraph<P>, bgraph_image<P, V> >
    super_;

    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    ///
    /// We use the associated type \c reference instead of a plain
    /// reference on the value type (\p V), because it's the only way
    /// to safely form a reference on the element in the case of a
    /// std::vector<bool>.
    typedef typename std::vector<V>::reference lvalue;

    /// Return type of read-only access.
    typedef typename std::vector<V>::const_reference rvalue;

    /// Skeleton.
    typedef bgraph_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    /// \{
    bgraph_image();
    bgraph_image(const p_bgraph<P>& g);
    bgraph_image(const p_bgraph<P>& g, const std::vector<V>& val);
    /// \}

    /// Initialize an empty image.
    void init_(const p_bgraph<P>& g, const std::vector<V>& val);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const bgraph_psite<P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const bgraph_psite<P>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_bgraph<P>& domain() const;

    /// Return the array of values associated to the nodes.
    const std::vector<V>& node_values() const;
    /// \}


    /// Return the point of the first node adjacent to the edge with
    /// id \a e.
    const P& node1(const typename p_bgraph<P>::edge_id& e) const;
    /// Return the point of the second node adjacent to the edge with
    /// id \a e.
    const P& node2(const typename p_bgraph<P>::edge_id& e) const;
};

  // Initialization routine.
  template <typename P, typename V>
  void init_(tag::image_t,
	     bgraph_image<P, V>& target, const bgraph_image<P, V>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <typename P, typename V>
  inline
  void init_(tag::image_t,
	     bgraph_image<P, V>& target, const bgraph_image<P, V>& model)
  {
    target.init_(model.domain(), model.node_values ());
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <typename P, typename V>
    inline
    data< bgraph_image<P, V> >::data(const p_bgraph<P>& g,
				       const std::vector<V>& val)
      : pg_ (g),
	val_ (val)
    {
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <typename P, typename V>
  inline
  bgraph_image<P, V>::bgraph_image()
  {
  }

  template <typename P, typename V>
  inline
  bgraph_image<P, V>::bgraph_image(const p_bgraph<P>& g)
  {
    init_(g, std::vector<V>(g.nsites()));
  }

  template <typename P, typename V>
  inline
  bgraph_image<P, V>::bgraph_image(const p_bgraph<P>& g,
				   const std::vector<V>& val)
  {
    init_(g, val);
  }

  template <typename P, typename V>
  inline
  void
  bgraph_image<P, V>::init_(const p_bgraph<P>& g, const std::vector<V>& val)
  {
    this->data_ = new internal::data< bgraph_image<P, V> > (g, val);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <typename P, typename V>
  inline
  typename bgraph_image<P, V>::rvalue
  bgraph_image<P, V>::operator()(const bgraph_psite<P>& p) const
  {
    mln_precondition(&p.pg().to_graph() == &this->data_->pg_.to_graph());
    mln_precondition(p.id() < this->data_->val_.size());
    return this->data_->val_[p.id()];
  }

  template <typename P, typename V>
  inline
  typename bgraph_image<P, V>::lvalue
  bgraph_image<P, V>::operator()(const bgraph_psite<P>& p)
  {
    mln_precondition(&p.pg().to_graph() == &this->data_->pg_.to_graph());
    mln_precondition(p.id() < this->data_->val_.size());
    return this->data_->val_[p.id()];
  }

  template <typename P, typename V>
  inline
  const std::vector<V>&
  bgraph_image<P, V>::node_values() const
  {
    return this->data_->val_;
  }

  template <typename P, typename V>
  inline
  const p_bgraph<P>&
  bgraph_image<P, V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->pg_;
  }

  template <typename P, typename V>
  inline
  const P&
  bgraph_image<P, V>::node1(const typename p_bgraph<P>::edge_id& e) const
  {
    return this->domain().node1(e);
  }

  template <typename P, typename V>
  inline
  const P&
  bgraph_image<P, V>::node2(const typename p_bgraph<P>::edge_id& e) const
  {
    return this->domain().node2(e);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_BGRAPH_IMAGE_HH
