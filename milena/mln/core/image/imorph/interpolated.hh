// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_IMORPH_INTERPOLATED_HH
# define MLN_CORE_IMAGE_IMORPH_INTERPOLATED_HH

/// \file
///
/// \brief Definition of a morpher that makes an image become readable
/// with floating coordinates.
///
/// \todo think having has(algebra::vec v) as a method


# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/algebra/vec.hh>
# include <mln/value/set.hh>

namespace mln
{

  // Forward declaration.
  template <typename I, template <class> class F> struct interpolated;

  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::interpolated<I, F>.
    */
    template <typename I, template <class> class F>
    struct data< interpolated<I,F> >
    {
      data(I& ima);

      I& ima_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, template <class> class F>
    struct image_< interpolated<I,F> >
      : public image_<I> // Same as I except...
    {
      // ...these changes.
      typedef trait::image::value_io::read_only value_io;
    };

  } // end of namespace mln::trait


  /// Makes the underlying image being accessed with floating coordinates.
  ///
  /// \ingroup modimageidentity
  //
  template <typename I, template <class> class F>
  struct interpolated :
    public mln::internal::image_identity< I, mln_domain(I), interpolated<I,F> >
  {

    typedef mln::internal::image_identity< I, mln_domain(I),
					   interpolated<I,F> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Return type of read-write access.
    typedef mln_lvalue(I) lvalue; // FIXME: Depends on lvalue presence in I.

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Skeleton.
    typedef interpolated< tag::image_<I>, F > skeleton;


    /// Constructors.
    ///FIXME: don't we want a 'const' here?
    interpolated(I& ima);
    interpolated();

    /// Initialize an empty image.
    void init_(I& ima);


    /// Test if this image has been initialized.
    bool is_valid() const;

    /// Test if a pixel value is accessible at \p p.
    using super_::has;

    /// Test if a pixel value is accessible at \p v.
    template <typename C>
    bool has(const mln::algebra::vec<I::psite::dim, C>& v) const;

    /// Read-only access of pixel value at point site \p p.
    /// Mutable access is only OK for reading (not writing).
    using super_::operator();

    mln_value(I) operator()(const mln::algebra::vec<psite::dim, float>& v) const;
    mln_value(I) operator()(const mln::algebra::vec<psite::dim, float>& v);

    const F<I> fun_;
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< interpolated<I,S> >

    template <typename I, template <class> class F>
    inline
    data< interpolated<I,F> >::data(I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal

  template <typename I, template <class> class F>
  inline
  interpolated<I,F>::interpolated(I& ima)
    : fun_(ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima);
  }

  template <typename I, template <class> class F>
  inline
  interpolated<I,F>::interpolated()
  {
  }

  template <typename I, template <class> class F>
  inline
  void
  interpolated<I, F >::init_(I& ima)
  {
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< interpolated<I,F> >(ima);
  }

  template <typename I, template <class> class F>
  inline
  bool interpolated<I,F>::is_valid() const
  {
    mln_invariant(this->data_->ima_.is_valid());
    return true;
  }

  template <typename I, template <class> class F>
  template <typename C>
  inline
  bool interpolated<I,F>::has(const mln::algebra::vec<I::psite::dim, C>& v) const
  {
    mln_psite(I) p;
    for (unsigned i = 0; i < I::psite::dim; ++i)
      p[i] = static_cast<int>(round(v[i]));
    return this->data_->ima_.has(p);
  }


  template <typename I, template <class> class F>
  inline
  mln_value(I)
  interpolated<I,F>::operator()(const mln::algebra::vec<psite::dim, float>& v) const
  {
    return fun_(v);
  }

  template <typename I, template <class> class F>
  inline
  mln_value(I)
  interpolated<I,F>::operator()(const mln::algebra::vec<psite::dim, float>& v)
  {
    return fun_(v);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_INTERPOLATED_HH
