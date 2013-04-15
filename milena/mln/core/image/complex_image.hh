// Copyright (C) 2008, 2009, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_COMPLEX_IMAGE_HH
# define MLN_CORE_IMAGE_COMPLEX_IMAGE_HH

/// \file
///
/// Definition of a line complex-based image.

# include <vector>

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/metal/vec.hh>
# include <mln/core/site_set/p_complex.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/value/set.hh>

// FIXME: Move this elsewhere.
// Working around std::vector<bool>'s (forced) specialization.

# define mlc_unbool(V)				\
   typename mln::internal::unbool<V>::ret

namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief A boolean proxy, used to fool std::vector.
    */
    struct bool_proxy
    {
    public:
      bool_proxy() {}
      bool_proxy(bool b) : b_(b) {}
      bool& operator=(bool b) { b_ = b; return *this; }

      operator bool&() { return b_; }
      operator const bool&() const { return b_; }

      bool operator==(const bool_proxy& rhs) { return b_ == rhs.b_; }
      bool operator< (const bool_proxy& rhs) { return b_ <  rhs.b_; }

    private:
      /// The value.
      bool b_;
    };

    template <typename V> struct unbool       { typedef V          ret; };
    template <>           struct unbool<bool> { typedef bool_proxy ret; };

  } // end of namespace mln::internal

}



/* FIXME: In the current implementation, the type of values on faces
   of different dimensions is necessarily the same (V).  We should
   allow different data types for vertices and edges.  */


namespace mln
{

  // Forward declaration.
  template <unsigned D, typename G, typename V> class complex_image;

  namespace internal
  {

    /*!
      \brief Data structure for \c mln::complex_image.
    */
    template <unsigned D, typename G, typename V>
    struct data< complex_image<D, G, V> >
    {
      data(const p_complex<D, G>& pc,
	   const metal::vec< D + 1, std::vector<V> >& values);

      metal::vec< D + 1, std::vector< mlc_unbool(V) > > values_;
      const p_complex<D, G> pc_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <unsigned D, typename G, typename V>
    struct image_< complex_image<D, G, V> >
      : default_image_< V, complex_image<D, G, V> >
    {
      typedef trait::image::category::primary category;

      // FIXME: Is that right?
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // Value.
      typedef trait::image::value_access::direct           value_access;
      // FIXME: This image is value wise read_write, and provides
      // an access to its values throught a multi-set,
      // but we need to add the corresponding interface in the class.
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_storage::disrupted       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::irrelevant    value_alignment;
      typedef trait::image::value_io::read_write           value_io;

      // Site / domain.
      typedef trait::image::pw_io::read_write              pw_io;
      typedef trait::image::localization::space            localization;
      /* FIXME: Depends on G.  We could use
	 `trait::image::space_from_point<mln_site(G)>::ret' in most
	 cases (i.e., when G's site is a Point), but would not be
	 generic.  */
      typedef typename trait::image::dimension::none    dimension;

      // Extended domain.
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait


  /// \brief Image based on a complex.
  ///
  /// Values attached to each face of the complex.
  ///
  /// \tparam D The dimension of the complex.
  /// \tparam G The geometry type of the complex.
  /// \tparam V The value type of the image.
  ///
  /// \ingroup modimageconcrete
  template <unsigned D, typename G, typename V>
  class complex_image
    : public internal::image_primary< V, p_complex<D, G>,
				      complex_image<D, G, V> >
  {
  public:
    /// The dimension of the complex.
    static const unsigned dim = D;
    /// The geometry type of the complex.
    typedef G geom;
    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    typedef V& lvalue;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Skeleton.
    typedef complex_image< D, tag::psite_<G>, tag::value_<V> > skeleton;

  public:
    /// Constructors.
    /// \{
    complex_image();
    complex_image(const p_complex<D, G>& pc);
    complex_image(const p_complex<D, G>& pc,
		  const metal::vec< D + 1, std::vector<V> >& values);
    /// \}

    /// \cond INTERNAL_API
    /// Initialize an empty image.
    void init_(const p_complex<D, G>& pc,
	       const metal::vec< D + 1, std::vector<V> >& values);
    /// \endcond

    /// Read-only access of face value at point site \p p.
    rvalue operator()(const complex_psite<D, G>& p) const;
    /// Read-write access of face value at point site \p p.
    lvalue operator()(const complex_psite<D, G>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_complex<D, G>& domain() const;

    /// Return the array of values associated to the faces.
    const metal::vec<D + 1, std::vector< mlc_unbool(V) > >& values() const;
    /// \}
  };

  /// \cond INTERNAL_API
  // Fwd decl.
  template <unsigned D, typename G, typename V, typename W>
  void init_(tag::image_t,
	     complex_image<D, G, V>& target,
	     const complex_image<D, G, W>& model);
  /// \endcond

# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <unsigned D, typename G, typename V, typename W>
  inline
  void init_(tag::image_t,
	     complex_image<D, G, V>& target,
	     const complex_image<D, G, W>& model)
  {
    metal::vec<D + 1, std::vector<V> > values;
    for (unsigned i = 0; i <= D; ++i)
      values[i].resize(model.domain().nfaces_of_dim(i));
    target.init_(model.domain(), values);
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <unsigned D, typename G, typename V>
    inline
    data< complex_image<D, G, V> >::data(const p_complex<D, G>& pc,
					 const metal::vec< D + 1, std::vector<V> >& values)
      : pc_(pc)
    {
      // We cannot use the initialization list for values_ (it
      // would not work when V = bool).
      for (unsigned i = 0; i <= D; ++i)
	{
	  values_[i].reserve(values[i].size());
	  values_[i].insert(values_[i].begin(),
			    values[i].begin(), values[i].end());
	}

      // Ensure the complex is consistent with the values.
      /* FIXME: We need additional macros in mln/core/contract.hh for
	 big blocks of preconditions like this one.  */
# ifndef NDEBUG
      for (unsigned i = 0; i < D; ++i)
	mln_precondition(pc.nfaces_of_dim(i) == values[i].size());
# endif // !NDEBUG
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <unsigned D, typename G, typename V>
  inline
  complex_image<D, G, V>::complex_image()
  {
  }

  template <unsigned D, typename G, typename V>
  inline
  complex_image<D, G, V>::complex_image(const p_complex<D, G>& pc)
  {
    metal::vec<D + 1, std::vector<V> > values;
    for (unsigned i = 0; i <= D; ++i)
      values[i].resize(pc.nfaces_of_dim(i));
    init_(pc, values);
  }

  template <unsigned D, typename G, typename V>
  inline
  complex_image<D, G, V>::complex_image(const p_complex<D, G>& pc,
					const metal::vec< D + 1,
                                                          std::vector<V> >& values)
  {
    init_(pc, values);
  }

  template <unsigned D, typename G, typename V>
  inline
  void
  complex_image<D, G, V>::init_(const p_complex<D, G>& pc,
				const metal::vec< D + 1, std::vector<V> >& values)
  {
    mln_precondition(! this->is_valid());
    this->data_ =
      new internal::data< complex_image<D, G, V> >(pc, values);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <unsigned D, typename G, typename V>
  inline
  typename complex_image<D, G, V>::rvalue
  complex_image<D, G, V>::operator()(const complex_psite<D, G>& p) const
  {
    mln_precondition(this->data_->pc_.has(p));
    return this->data_->values_[p.n()][p.face_id()];
  }

  template <unsigned D, typename G, typename V>
  inline
  typename complex_image<D, G, V>::lvalue
  complex_image<D, G, V>::operator()(const complex_psite<D, G>& p)
  {
    mln_precondition(this->data_->pc_.has(p));
    return this->data_->values_[p.n()][p.face_id()];
  }

  template <unsigned D, typename G, typename V>
  inline
  const metal::vec< D + 1, std::vector< mlc_unbool(V) > >&
  complex_image<D, G, V>::values() const
  {
    return this->data_->values_;
  }

  template <unsigned D, typename G, typename V>
  inline
  const p_complex<D, G>&
  complex_image<D, G, V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->pc_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# undef mlc_unbool

#endif // ! MLN_CORE_IMAGE_COMPLEX_IMAGE_HH
