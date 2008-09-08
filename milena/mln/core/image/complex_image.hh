// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_COMPLEX_IMAGE_HH
# define MLN_CORE_IMAGE_COMPLEX_IMAGE_HH

/// \file mln/core/image/complex_image.hh
/// \brief Definition of a line complex-based image.

# include <vector>

# include <mln/trait/images.hh>

# include <mln/core/internal/image_primary.hh>
# include <mln/metal/vec.hh>
# include <mln/core/site_set/p_complex.hh>
# include <mln/core/complex_psite.hh>
# include <mln/value/set.hh>


/* FIXME: In the current implementation, the type of values on faces
   of different dimensions is necessarily the same (V).  We should
   allow different data types for vertices and edges.  */


namespace mln
{

  // Fwd decl.
  template <unsigned D, typename P, typename V> struct complex_image;

  namespace internal
  {

    /// \internal Data structure for \c mln::complex_image<P,V>.
    template <unsigned D, typename P, typename V>
    struct data_< complex_image<D, P, V> >
    {
      data_(const p_complex<D, P>& pc,
	    const metal::vec< D + 1, std::vector<V> >& values);

      metal::vec< D + 1, std::vector<V> > values_;
      const p_complex<D, P> pc_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <unsigned D, typename P, typename V>
    struct image_< complex_image<D, P, V> >
      : default_image_< V, complex_image<D, P, V> >
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


  /// \brief Image based on a complex.
  ///
  /// Values are stored on the vertices of the graph.
  template <unsigned D, typename P, typename V>
  struct complex_image :
    public internal::image_primary_< V, p_complex<D, P>, complex_image<D,P,V> >
  {
    /// Super type.
    typedef mln::internal::image_base_< p_complex<D, P>,
					complex_image<D, P, V> > super_;

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

    /// Skeleton.
    typedef complex_image< D, tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    /// \{
    complex_image();
    complex_image(const p_complex<D, P>& pc);
    complex_image(const p_complex<D, P>& pc,
		  const metal::vec< D + 1, std::vector<V> >& values);
    /// \}

    /// Initialize an empty image.
    void init_(const p_complex<D, P>& pc,
	       const metal::vec< D + 1, std::vector<V> >& values);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const complex_psite<D, P>& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const complex_psite<D, P>& p);

    /// Accessors.
    /// \{
    /// Return the domain of psites od the image.
    const p_complex<D, P>& domain() const;

    /// Return the array of values associated to the faces.
    const metal::vec<D + 1, std::vector<V> >& face_values() const;
    /// \}
  };

  // Fwd decl.
  template <unsigned D, typename P, typename V, typename W>
  void init_(tag::image_t,
	     complex_image<D, P, V>& target,
	     const complex_image<D, P, W>& model);


# ifndef MLN_INCLUDE_ONLY

  /*-----------------.
  | Initialization.  |
  `-----------------*/

  template <unsigned D, typename P, typename V, typename W>
  inline
  void init_(tag::image_t,
	     complex_image<D, P, V>& target,
	     const complex_image<D, P, W>& model)
  {
    metal::vec<D + 1, std::vector<V> > values;
    for (unsigned i = 0; i < D; ++i)
      values[i].resize(model.values[i].size());
    target.init_(model.domain(), values);
  }

  /*-------.
  | Data.  |
  `-------*/

  namespace internal
  {
    template <unsigned D, typename P, typename V>
    inline
    data_< complex_image<D, P, V> >::data_(const p_complex<D, P>& pc,
					   const metal::vec< D + 1,
					                     std::vector<V> >& values)
      : values_(values),
	pc_(pc)
    {
      // Ensure the complex is consistent with the values.
      /* FIXME: We need additional macros in mln/core/contract.hh for
	 big blocks of preconditions like this one.  */
# ifndef NDEBUG
      for (unsigned i = 0; i < D; ++i)
	mln_precondition(pc.cplx().nfaces(i) == values[i].size());
# endif // !NDEBUG
    }

  } // end of namespace mln::internal

  /*---------------.
  | Construction.  |
  `---------------*/

  template <unsigned D, typename P, typename V>
  inline
  complex_image<D, P, V>::complex_image()
  {
  }

  template <unsigned D, typename P, typename V>
  inline
  complex_image<D, P, V>::complex_image(const p_complex<D, P>& pc)
  {
    metal::vec<D + 1, std::vector<V> > values;
    for (unsigned i = 0; i < D; ++i)
      values[i].resize(pc.cplx().nfaces(i));
    init_(pc, values);
  }

  template <unsigned D, typename P, typename V>
  inline
  complex_image<D, P, V>::complex_image(const p_complex<D, P>& pc,
					const metal::vec< D + 1,
                                                          std::vector<V> >& values)
  {
    init_(pc, values);
  }

  template <unsigned D, typename P, typename V>
  inline
  void
  complex_image<D, P, V>::init_(const p_complex<D, P>& pc,
				const metal::vec< D + 1, std::vector<V> >& values)
  {
    mln_precondition(! this->has_data());
    this->data_ =
      new internal::data_< complex_image<D, P, V> >(pc, values);
  }

  /*---------------.
  | Manipulation.  |
  `---------------*/

  template <unsigned D, typename P, typename V>
  inline
  typename complex_image<D, P, V>::rvalue
  complex_image<D, P, V>::operator()(const complex_psite<D, P>& p) const
  {
    mln_precondition(this->data_->pc_.has(p));
    return this->data_->values_[p.n()][p.face_id()];
  }

  template <unsigned D, typename P, typename V>
  inline
  typename complex_image<D, P, V>::lvalue
  complex_image<D, P, V>::operator()(const complex_psite<D, P>& p)
  {
    mln_precondition(this->data_->pc_.has(p));
    return this->data_->values_[p.n()][p.face_id()];
  }

  template <unsigned D, typename P, typename V>
  inline
  const metal::vec< D + 1, std::vector<V> >&
  complex_image<D, P, V>::face_values() const
  {
    return this->data_->val_;
  }

  template <unsigned D, typename P, typename V>
  inline
  const p_complex<D, P>&
  complex_image<D, P, V>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->pc_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_COMPLEX_IMAGE_HH
