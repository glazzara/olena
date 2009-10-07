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

#ifndef MLN_CORE_IMAGE_ATTRIBUTE_IMAGE_HH
# define MLN_CORE_IMAGE_ATTRIBUTE_IMAGE_HH

# include <mln/core/internal/image_primary.hh>
# include <mln/core/concept/tree.hh>
# include <mln/metal/converts_to.hh>

namespace mln
{
  // Forward declaration.
  template <typename T, typename V> class attribute_image;

  namespace internal
  {

    /// Data structure for \c mln::attribute_image
    template <typename T, typename V>
    struct data< mln::attribute_image<T, V> >
    {
      data();
      data(const T& tree);
      ~data();

      std::vector<V> values_;
      T tree_; // An attribute image can hold the tracked-data of the tree.
    };

  } // end of namespace mln::internal

  namespace trait
  {

    template <typename T, typename V>
    struct image_< attribute_image<T, V> > : default_image_< V, attribute_image<T, V> >
    {
      // misc
      typedef trait::image::category::primary	category;
      typedef trait::image::speed::fastest	speed;
      typedef trait::image::size::regular	size;

      // value
      typedef trait::image::vw_io::none				vw_io;
      typedef trait::image::vw_set::none			vw_set;
      typedef trait::image::value_access::direct		value_access;
      typedef trait::image::value_alignment::irrelevant		value_alignment;
      typedef trait::image::value_browsing::site_wise_only	value_browsing;
      typedef trait::image::value_storage::one_block		value_storage;
      typedef trait::image::value_io::read_write		value_io;

      // site / domain
      typedef trait::image::pw_io::read_write		pw_io;
      typedef trait::image::localization::none		localization;
      typedef trait::image::dimension::none		dimension;

      // extended domain
      typedef trait::image::ext_domain::none		ext_domain;
      typedef trait::image::ext_value::irrelevant	ext_value;
      typedef trait::image::ext_io::irrelevant		ext_io;
    };

  } // end of namespace mln::trait


  namespace convert
  {

    namespace over_load
    {

      template <typename T, typename V, typename I>
      void
      from_to_(const mln::attribute_image<T, V>& from, Image<I>& to_)
      {
	typedef mln::attribute_image<T, V> A;
	I& to = exact(to_);

	mlc_converts_to(V, mln_value(I))::check();
	mlc_converts_to(typename T::domain_t, mln_domain(I))::check();

	mln_precondition(from.is_valid());
	to.init_(from.tree().domain());

	mln_fwd_piter(I) p(to.domain());
	for_all(p)
	  convert::from_to(from(from.tree().node(p)), to(p));
      }

    } // enf of namespace mln::convert::over_load

  } // end of namespace mln::convert


  /// Basic attribute image class.
  ///
  /// The parameter \c V is the type of pixel values (usually the
  /// result type of the attribute).  This image class stores data in
  /// memory.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T, typename V>
  class attribute_image : public internal::image_primary< V, typename T::nodes_t, attribute_image<T, V> >
  {
    typedef internal::image_primary< V, typename T::nodes_t, attribute_image<T, V> > super_;
    typedef attribute_image<T, V> self_;

  public:

    /// Value associated type.
    typedef V		value;
    /// Return type of read-only access.
    typedef const V&	rvalue;
    /// Return type of read-write access.
    typedef V&		lvalue;


    /// Site (node) assocatied type.
    typedef typename T::node_t	node_t;
    typedef node_t		site;
    typedef node_t		psite;

    /// Site set (node set) associated type.
    typedef typename T::nodes_t nodes_t;

    /// Skeleton.
    typedef attribute_image< T, tag::value_<V> > skeleton;

    /// Constructors
    /// \{
    /// Constructor without argument.
    attribute_image();
    /// Allocate an attribute image respecting the size of the tree.
    attribute_image(const Tree<T>& tree);
    /// \}

    /// Initialize an empty image.
    void init_(const Tree<T>& tree);

    /// Test if the node \p n is valid.
    bool has(const node_t& n) const;

    /// Give the node site set.
    const nodes_t& domain() const;

    /// Read-only access to the image value at node \p n.
    rvalue operator()(const node_t& n) const;

    /// Read-write access to the image value at node \p n.
    lvalue operator()(const node_t& n);

    // Specific methods:
    // ----------------

    /// Read access to the value at point \p p.
    /// Note: can be ambigous if the node content is a node set.
    rvalue operator()(const mln_psite(T)& p) const;

    /// Return the tree.
    const T& tree() const;

    /// Change the target tree.
    void change_tree(const Tree<T>& tree);

    // As a fastest image.
    // ------------------

    /// Give the number of elements.
    unsigned nelements() const;

    /// Read-only access to the image value lacated at index \p i.
    rvalue element(unsigned i) const;

    /// Read-write access to the image value lacated at index \p i.
    lvalue element(unsigned i);

    /// Return the border. (Required by image fastest / return always 0).
    unsigned border() const;

    /// Give the delta_index corresponding to the delta point.
    int delta_index(const mln_delta(node_t)& dp) const;

    /// Give the point corresponding to the index \p i;
    node_t point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const value* buffer() const;

    /// Give a hook to the value buffer.
    value* buffer();
  };

  // Forward declaration.
  template <typename T, typename V, typename V2>
  void init_(tag::image_t, mln::attribute_image<T, V>& target, const mln::attribute_image<T, V2>& model);


# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T, typename V, typename V2>
  inline
  void
  init_(tag::image_t, mln::attribute_image<T, V>& target, const mln::attribute_image<T, V2>& model)
  {
    target.init_(model.tree());
  }

  // internal::data

  namespace internal
  {

    template <typename T, typename V>
    inline
    data< attribute_image<T, V> >::data()
    {
    }

    template <typename T, typename V>
    inline
    data< attribute_image<T, V> >::data(const T& tree)
      : values_ (tree.n_nodes()),
	tree_ (tree)
    {
    }

    template <typename T, typename V>
    inline
    data< attribute_image<T, V> >::~data()
    {
    }

  } // end of namespace mln::internal


  template <typename T, typename V>
  inline
  attribute_image<T, V>::attribute_image()
  {
  }


  template <typename T, typename V>
  inline
  attribute_image<T, V>::attribute_image(const Tree<T>& tree_)
  {
    const T& tree = exact(tree_);
    mln_precondition(tree.is_valid());
    this->data_ = new internal::data< self_ >(tree);
  }

  template <typename T, typename V>
  inline
  unsigned
  attribute_image<T, V>::border() const
  {
    return 0;
  }

  template <typename T, typename V>
  inline
  void
  attribute_image<T, V>::init_(const Tree<T>& tree_)
  {
    const T& tree = exact(tree_);
    mln_precondition(tree.is_valid());
    this->data_->tree_ = tree;
    this->data_->values_.resize(tree.n_nodes());
  }

  template <typename T, typename V>
  inline
  bool
  attribute_image<T, V>::has(const node_t& n) const
  {
    mln_precondition(this->is_valid());
    return this->domain().has(n);
  }

  template <typename T, typename V>
  inline
  const typename T::nodes_t&
  attribute_image<T, V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->tree_.nodes();
  }

  template <typename T, typename V>
  inline
  const V&
  attribute_image<T, V>::operator()(const node_t& n) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->has(n));
    return this->data_->values_[n.index()];
  }

  template <typename T, typename V>
  inline
  V&
  attribute_image<T, V>::operator()(const node_t& n)
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->has(n));
    return this->data_->values_[n.index()];
  }

  // Specific methods:
  // ----------------


  template <typename T, typename V>
  inline
  const V&
  attribute_image<T, V>::operator()(const mln_psite(T)& p) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->tree_.has(p));
    return this->data_->values_[this->data_->tree_.node_at_(p)];
  }

  template <typename T, typename V>
  inline
  const T&
  attribute_image<T, V>::tree() const
  {
    mln_precondition(this->is_valid());
    return this->data_->tree_;
  }

  template <typename T, typename V>
  inline
  void
  attribute_image<T, V>::change_tree(const Tree<T>& tree_)
  {
    const T& tree = exact(tree_);
    mln_precondition(tree.is_valid());
    this->data_->tree_ = tree;
  }

  template <typename T, typename V>
  inline
  unsigned
  attribute_image<T, V>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->domain().nsites();
  }

  template <typename T, typename V>
  inline
  const V&
  attribute_image<T, V>::element(unsigned i) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(i < this->nelements());
    return this->data_->values_[i];
  }

  template <typename T, typename V>
  inline
  V&
  attribute_image<T, V>::element(unsigned i)
  {
    mln_precondition(this->is_valid());
    mln_precondition(i < this->nelements());
    return this->data_->values_[i];
  }

  template <typename T, typename V>
  inline
  int
  attribute_image<T, V>::delta_index(const mln_delta(node_t)& dp) const
  {
    mln_precondition(this->is_valid());
    return dp.value();
  }

  template <typename T, typename V>
  inline
  typename T::node_t
  attribute_image<T, V>::point_at_index(unsigned i) const
  {
    mln_precondition(this->is_valid());
    node_t tmp(this->data_->tree_, i);
    return tmp;
  }

  template <typename T, typename V>
  inline
  const V*
  attribute_image<T, V>::buffer() const
  {
    return &(this->data_->values_[0]);
  }

  template <typename T, typename V>
  inline
  V* attribute_image<T, V>::buffer()
  {
    return &(this->data_->values_[0]);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_CORE_IMAGE_ATTRIBUTE_IMAGE_HH
