// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_HH
# define MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_HH

/// \file mln/core/image/imorph/labeled_image.hh
///
/// Definition of a morpher on a labeled image.

# include <mln/core/image/dmorph/image_if.hh>

# include <mln/core/concept/function.hh>

# include <mln/core/internal/image_identity.hh>

# include <mln/core/site_set/box.hh>

# include <mln/accu/pair.hh>
# include <mln/accu/nil.hh>
# include <mln/accu/center.hh>

# include <mln/labeling/compute.hh>
# include <mln/labeling/pack.hh>
# include <mln/labeling/relabel.hh>

# include <mln/util/array.hh>

# ifndef NDEBUG
#  include <mln/accu/max.hh>
#  include <mln/level/compute.hh>
# endif // ! NDEBUG


namespace mln
{

  // Forward declarations.
  template <typename I> struct labeled_image;
  namespace accu
  {
    template <typename T> struct nil;
    template <typename T> struct bbox;
  }


  namespace internal
  {

    /// Data structure for \c mln::labeled_image<I>.
    template <typename I>
    struct data< labeled_image<I> >
    {
      data(const I& ima, const mln_value(I)& nlabels);

      I ima_;
      mln_value(I) nlabels_;
      mutable util::array< box<mln_psite(I)> > bboxes_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< labeled_image<I> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::indirect value_access;

      typedef trait::image::value_io::read_only value_io;
      typedef trait::image::pw_io::read pw_io;

      // extended domain
      typedef trait::image::ext_value::multiple  ext_value;
      typedef trait::image::ext_io::read_only    ext_io;
    };

  } // end of namespace mln::trait



  /// Morpher providing an improved interface for labeled image.
  ///
  /// \tparam I The label image type.
  ///
  /// This image type allows to access every site set at a given
  /// label.
  ///
  /// This image type guaranties that labels are contiguous (from 1 to
  /// n).
  ///
  /// \ingroup modimageidmorpher
  //
  template <typename I>
  class labeled_image
    : public internal::image_identity< const I, mln_domain(I), labeled_image<I> >
  {
    typedef internal::image_identity< const I, mln_domain(I), labeled_image<I> >
	    super_;

  public:

    /// Skeleton.
    typedef labeled_image< tag::image_<I> > skeleton;

    /// Type of the bounding component bounding boxes.
    typedef mln_result(accu::bbox<mln_psite(I)>) bbox_t;

    /// Constructors
    /// @{
    /// Constructor without argument.
    labeled_image();

    /// Constructor from an image \p ima and the number of labels \p nlabels.
    labeled_image(const I& ima, const mln_value(I)& nlabels);
    /// @}

    /// Deferred initialization from a labeled image \p ima and the number
    /// of labels \p nlabels.
    void init_(const I& ima, const mln_value(I)& nlabels);

    /// Duplicate the underlying image and create a new labeled_image.
    void init_from_(const labeled_image<I>& model);

    /// Relabel according to a function.
    /// @{
    //
    /// Labels may be removed and the labeling may not be contiguous
    /// afterwards.
    /// FIXME: currently the labels are packed after relabeling for
    /// performance reasons. Do we want to be less restrictive?
    /// \sa pack_().
    template <typename F>
    void relabel(const Function_v2v<F>& f);
    //
    /// Labels may be removed. This overload make sure the labeling is still
    /// contiguous.
    template <typename F>
    void relabel(const Function_v2b<F>& f);
    /// @}

    /// Pack labeling. Relabel if the labeling is not contiguous.
    void pack_();

    /// Return the number of labels;
    mln_value(I) nlabels() const;

    /// Update bounding boxes information.
    void update_();

    /// Return the bounding box of the component \p label.
    const bbox_t& bbox(const mln_value(I)& label) const;

    const util::array<bbox_t>& bboxes() const
    {
      return this->data_->bboxes_;
    }

      /// Return the domain of the component with label \p label.
      /// This is an optimized version.
//      p_if<mln_psite(I)> domain(const mln_value(I)& label) const;
  };


  // init_

  //FIXME: not enough generic? We would like 'J' instead of
  // 'labeled_image<I>'.
  template <typename I, typename J>
  void init_(tag::image_t, labeled_image<I>& target,
			   const labeled_image<J>& model);



  namespace make
  {

    template <typename I>
    mln::labeled_image<I>
    labeled_image(const Image<I>& ima, const mln_value(I)& nlabels);

  } // end of namespace mln::make




# ifndef MLN_INCLUDE_ONLY


  // internal::data< labeled_image<I> >

  namespace internal
  {


    // data< labeled_image<I> >

    template <typename I>
    inline
    data< labeled_image<I> >::data(const I& ima, const mln_value(I)& nlabels)
      : ima_(ima), nlabels_(nlabels)
    {
    }

  } // end of namespace mln::internal


  template <typename I>
  inline
  labeled_image<I>::labeled_image()
  {
  }

  template <typename I>
  inline
  labeled_image<I>::labeled_image(const I& ima, const mln_value(I)& nlabels)
  {
    init_(ima, nlabels);
  }

  template <typename I>
  inline
  void
  labeled_image<I>::init_(const I& ima, const mln_value(I)& nlabels)
  {
    mln_precondition(level::compute(accu::meta::max(), ima) == nlabels);
    this->data_ = new internal::data< labeled_image<I> >(ima, nlabels);
    this->update_();
  }

  template <typename I>
  inline
  void
  labeled_image<I>::init_from_(const labeled_image<I>& model)
  {
    this->data_
      = new internal::data< labeled_image<I> >(duplicate(model.hook_data_()->ima_),
						 model.nlabels());
    this->data_->bboxes_ = model.hook_data_()->bboxes_;
  }

  template <typename I>
  template <typename F>
  inline
  void
  labeled_image<I>::relabel(const Function_v2v<F>& f_)
  {
    const F& f = exact(f_);
    labeling::relabel_inplace(this->data_->ima_, this->data_->nlabels_, f);

    /// We MUST be sure that the labeling is contiguous in order to compute
    /// attributes.
    ///FIXME: do we want to be less restrictive?
    pack_();

    /// FIXME: could be highly improved: reorder the attributes according to
    /// the function f.
    this->update_();
  }

  template <typename I>
  template <typename F>
  inline
  void
  labeled_image<I>::relabel(const Function_v2b<F>& f_)
  {
    const F& f = exact(f_);
    labeling::relabel_inplace(this->data_->ima_, this->data_->nlabels_, f);

    /// FIXME: could be highly improved: reorder the attributes according to
    /// the function f.
    this->update_();
  }

  template <typename I>
  inline
  void
  labeled_image<I>::pack_()
  {
    labeling::pack_inplace(this->data_->ima_, this->data_->nlabels_);

    /// FIXME: could be highly improved: reorder the attributes according to
    /// the way the labels are packed.
    this->update_();
  }


  template <typename I>
  inline
  mln_value(I)
  labeled_image<I>::nlabels() const
  {
    return this->data_->nlabels_;
  }


  // init_

  template <typename I, typename J>
  void init_(tag::image_t, labeled_image<I>& target,
	     const labeled_image<J>& model)
  {
    I ima;
    init_(tag::image, ima, model);
    target.init_(ima, model.nlabels());
  }


  template <typename I>
  void
  labeled_image<I>::update_()
  {
    this->data_->bboxes_
      = labeling::compute(accu::meta::bbox(), this->data_->ima_,
	  this->data_->nlabels_);
  }


  template <typename I>
  const typename labeled_image<I>::bbox_t&
  labeled_image<I>::bbox(const mln_value(I)& label) const
  {
    return this->data_->bboxes_[label];
  }


//    template <typename I, typename V, typename E>
////    p_if<mln_psite(I)>
//    unsigned
//    extended_impl_selector<I,
//			   accu::pair<accu::bbox<mln_psite(I)>,
//				      accu::center<mln_psite(I),V> >,
//			   E>::domain(const mln_value(I)& label) const
//    {
//      const E& ima = internal::force_exact<E>(*this);
//      return ((ima.hook_data_() | bbox(label))
//	      | (pw::value(ima.hook_data_()) == pw::cst(label))).domain();
//    }


  // Make routines.

  namespace make
  {

    template <typename I>
    mln::labeled_image<I>
    labeled_image(const Image<I>& ima, const mln_value(I)& nlabels)
    {
      mln_precondition(exact(ima).is_valid());
      mln::labeled_image<I> tmp(exact(ima), nlabels);
      return tmp;
    }

  } // end of namespace mln::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_HH
