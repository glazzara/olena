// Copyright (C) 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_DMORPH_SLICE_IMAGE_HH
# define MLN_CORE_IMAGE_DMORPH_SLICE_IMAGE_HH

/// \file
///
/// Definition of a 2D image extracted from a slice of a 3D image.
///
/// \todo Write init_.

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/alias/box3d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/data/paste.hh>


namespace mln
{

  // Forward declaration.
  template <typename I> struct slice_image;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::slice_image<I>.
    */
    template <typename I>
    struct data< slice_image<I> >
    {
      data(I& ima, def::coord sli);

      I ima_;
      def::coord sli_;
      box2d b_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< slice_image<I> > : default_image_morpher< I,
							     mln_value(I),
							     slice_image<I> >
    {
      typedef trait::image::category::domain_morpher category;

      typedef trait::image::dimension::two_d      dimension;

//       typedef trait::image::ext_domain::none      ext_domain;  // No extension of domain.
//       typedef trait::image::ext_value::irrelevant ext_value;
//       typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_alignment::not_aligned   value_alignment;
      typedef trait::image::value_storage::disrupted       value_storage;
    };

  } // end of namespace mln::trait



  /// 2D image extracted from a slice of a 3D image.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I>
  struct slice_image : public internal::image_domain_morpher< I,
							      box2d,
							      slice_image<I> >
  {
    /// Skeleton.
    typedef slice_image< tag::image_<I> > skeleton;

    /// Constructor without argument.
    slice_image();

    /// Constructor from an image \p ima and a predicate \p f.
    slice_image(I& ima, def::coord sli);

    /// \cond INTERNAL_API
    void init_(I& ima, def::coord sli);
    /// \endcond

    template <typename V>
    void operator=(const image2d<V>& rhs);

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the slice number.
    def::coord sli() const;

    /// Read-only access to the image value located at point \p p.
    mln_rvalue(I) operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    mln_morpher_lvalue(I) operator()(const point2d& p);


    /// Const promotion via conversion.
    operator slice_image<const I>() const;
  };



  template <typename I>
  slice_image<I>
  slice(Image<I>& ima, def::coord sli);

  template <typename I>
  slice_image<const I>
  slice(const Image<I>& ima, def::coord sli);




  namespace trait
  {

    template <typename I, typename V>
    struct ch_value< slice_image<I>, V >
    {
      typedef image2d<V> ret;
    };

  } // end of namespace mln::trait



# ifndef MLN_INCLUDE_ONLY


//   // init_.

//   template <typename I, typename J>
//   void init_(tag::image_t, slice_image<I>& target, const J& model)
//   {
//     I ima;
//     init_(tag::image, ima, exact(model));
//     def::coord sli;
//     // FIXME
//     // init_(tag::???, sli, exact(model));
//     target.init_(ima, sli);
//   }


  // internal::data< slice_image<I> >

  namespace internal
  {

    template <typename I>
    inline
    data< slice_image<I> >::data(I& ima, def::coord sli)
      : ima_(ima),
	sli_(sli)
    {
      b_ = make::box2d(ima.domain().pmin().row(), ima.domain().pmin().col(),
		       ima.domain().pmax().row(), ima.domain().pmax().col());
    }

  }


  // slice_image<I>

  template <typename I>
  inline
  slice_image<I>::slice_image()
  {
  }

  template <typename I>
  inline
  slice_image<I>::slice_image(I& ima, def::coord sli)
  {
    init_(ima, sli);
  }

  template <typename I>
  inline
  void
  slice_image<I>::init_(I& ima, def::coord sli)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< slice_image<I> >(ima, sli);
  }

  template <typename I>
  template <typename V>
  void
  slice_image<I>::operator=(const image2d<V>& rhs)
  {
    data::paste(rhs, *this);
  }

  template <typename I>
  inline
  const box2d&
  slice_image<I>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename I>
  inline
  def::coord
  slice_image<I>::sli() const
  {
    mln_precondition(this->is_valid());
    return this->data_->sli_;
  }

  template <typename I>
  inline
  mln_rvalue(I)
  slice_image<I>::operator()(const point2d& p) const
  {
    mln_precondition(this->has(p));
    point3d p_(this->data_->sli_, p.row(), p.col());
    mln_precondition(this->data_->ima_.has(p_));
    return this->data_->ima_(p_);
  }

  template <typename I>
  inline
  mln_morpher_lvalue(I)
  slice_image<I>::operator()(const point2d& p)
  {
    mln_precondition(this->has(p));
    point3d p_(this->data_->sli_, p.row(), p.col());
    mln_precondition(this->data_->ima_.has(p_));
    return this->data_->ima_(p_);
  }

  template <typename I>
  inline
  slice_image<I>::operator slice_image<const I>() const
  {
    mln_precondition(this->is_valid());
    slice_image<const I> tmp(this->data_->ima_,
			     this->data_->sli_);
    return tmp;
  }


  // Routines.

  template <typename I>
  inline
  slice_image<I>
  slice(Image<I>& ima_, def::coord sli)
  {
    mlc_equal(mln_domain(I), box3d)::check();

    I& ima = exact(ima_);
    mln_precondition(ima.is_valid());
    mln_precondition(sli >= ima.domain().pmin().sli() &&
		     sli <= ima.domain().pmax().sli());

    slice_image<I> tmp(ima, sli);
    return tmp;
  }

  template <typename I>
  inline
  slice_image<const I>
  slice(const Image<I>& ima_, def::coord sli)
  {
    mlc_equal(mln_domain(I), box3d)::check();

    const I& ima = exact(ima_);
    mln_precondition(ima.is_valid());
    mln_precondition(sli >= ima.domain().pmin().sli() &&
		     sli <= ima.domain().pmax().sli());

    slice_image<const I> tmp(ima, sli);
    return tmp;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_IMAGE_DMORPH_SLICE_IMAGE_HH
