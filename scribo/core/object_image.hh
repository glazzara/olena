// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_OBJECT_IMAGE_HH
# define SCRIBO_CORE_OBJECT_IMAGE_HH

/// \file
///
/// \brief Definition of a morpher on a labeled image.

# include <mln/core/internal/labeled_image_base.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/util/array.hh>

# include <mln/data/compute.hh>

# include <mln/accu/center.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/accu/stat/max.hh>

# include <mln/labeling/compute.hh>

namespace mln
{

  // Forward declarations.
  template <typename I> struct object_image;

  namespace internal
  {
    using namespace mln;

    /// Data structure for \c mln::object_image<I>.
    template <typename I>
    struct data< object_image<I> >
      : data< labeled_image_base<I, object_image<I> > >
    {
      typedef data< labeled_image_base<I, object_image<I> > > super_;
      typedef mln_psite(I) ms_t;
      typedef mln_result(accu::center<mln_site(I)>) accu_res_t;

      data(const I& ima, const mln_value(I)& nlabels);
      data(const I& ima, const mln_value(I)& nlabels,
	   const util::array<mln_box(I)>& bboxes,
	   const util::array<ms_t>& mass_centers);
      data(const I& ima, const mln_value(I)& nlabels,
	   const util::array<mln_box(I)>& bboxes,
	   const util::array<accu_res_t>& mass_centers);

      mutable util::array<ms_t> mass_centers_;

      util::array<accu::center<mln_psite(I)> > tmp_accus_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< object_image<I> >
      : image_< labeled_image_base< I, object_image<I> > >
    {
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
  class object_image
    : public mln::labeled_image_base<I, object_image<I> >
  {
    typedef mln::labeled_image_base<I, object_image<I> > super_;

  public:

    /// Skeleton.
    typedef object_image< tag::image_<I> > skeleton;

    /// Mass center type.
    typedef typename internal::data< object_image<I> >::ms_t ms_t;
    typedef mln_result(accu::center<mln_site(I)>) accu_res_t;

    /// Constructors
    /// @{
    /// Constructor without argument.
    object_image();

    /// Constructor from an image \p ima and the number of labels \p nlabels.
    object_image(const I& ima, const mln_value(I)& nlabels);

    /// Constructor from an image \p ima, the number of labels \p
    /// nlabels and the object bounding boxes.
    template <typename P>
    object_image(const I& ima, const mln_value(I)& nlabels,
		 const util::array<mln_box(I)>& bboxes,
		 const util::array<P>& mass_centers);
    /// @}

    /// Deferred initialization from a labeled image \p ima and the number
    /// of labels \p nlabels.
    void init_(const I& ima, const mln_value(I)& nlabels);

    /// Duplicate the underlying image and create a new object_image.
    void init_from_(const object_image<I>& model);

    /// Return an array of mass centers.
    const util::array<ms_t>& mass_centers() const;

    /// Return the mass center of an object with label \p label.
    const mln_psite(I)& mass_center(const mln_value(I)& label) const;

    /// Return the underlying labeled image
    const I& labeled_image_() const;

    /// Update mass centers when objects are merged.
    void init_update_data_();
    void prepare_update_data_(const mln_value(I)& lbl,
			      const mln_value(I)& new_lbl);
    void update_data_(const fun::i2v::array<mln_value(I)>& relabel_fun);
  };


  // init_

  //FIXME: not enough generic? We would like 'J' instead of
  // 'object_image<I>'.
  template <typename I, typename J>
  void init_(tag::image_t, object_image<I>& target,
	     const object_image<J>& model);



  namespace make
  {

    template <typename I>
    mln::object_image<I>
    object_image(const Image<I>& ima, const mln_value(I)& nlabels);

  } // end of namespace mln::make




# ifndef MLN_INCLUDE_ONLY


  // internal::data< object_image<I> >

  namespace internal
  {


    // data< object_image<I> >

    template <typename I>
    inline
    data< object_image<I> >::data(const I& ima, const mln_value(I)& nlabels)
      : super_(ima, nlabels)
    {
    }

    template <typename I>
    inline
    data< object_image<I> >::data(const I& ima, const mln_value(I)& nlabels,
				  const util::array<mln_box(I)>& bboxes,
				  const util::array<ms_t>& mass_centers)
      : super_(ima, nlabels, bboxes), mass_centers_(mass_centers)
    {
    }


    template <typename I>
    inline
    data< object_image<I> >::data(const I& ima, const mln_value(I)& nlabels,
				  const util::array<mln_box(I)>& bboxes,
				  const util::array<accu_res_t>& mass_centers)
      : super_(ima, nlabels, bboxes)
    {
      convert::from_to(mass_centers, mass_centers_);
    }


  } // end of namespace mln::internal


  template <typename I>
  inline
  object_image<I>::object_image()
  {
  }

  template <typename I>
  inline
  object_image<I>::object_image(const I& ima, const mln_value(I)& nlabels)
  {
    init_(ima, nlabels);
  }

  template <typename I>
  template <typename P>
  inline
  object_image<I>::object_image(const I& ima, const mln_value(I)& nlabels,
				const util::array<mln_box(I)>& bboxes,
				const util::array<P>& mass_centers)
  {
    mln_precondition(data::compute(accu::meta::stat::max(), ima) == nlabels);
    this->data_ = new internal::data< object_image<I> >(ima, nlabels,
							bboxes, mass_centers);
  }


  template <typename I>
  inline
  void
  object_image<I>::init_(const I& ima, const mln_value(I)& nlabels)
  {
    mln_precondition(data::compute(accu::meta::stat::max(), ima) == nlabels);
    this->data_ = new internal::data< object_image<I> >(ima, nlabels);

    // FIXME: could be improved!
    this->data_->bboxes_ = labeling::compute(accu::meta::shape::bbox(),
					     this->data_->ima_,
					     this->data_->nlabels_);
    convert::from_to(labeling::compute(accu::meta::center(),
				       this->data_->ima_,
				       this->data_->nlabels_),
		     this->data_->mass_centers_);
  }


  template <typename I>
  inline
  void
  object_image<I>::init_from_(const object_image<I>& model)
  {
    typedef internal::data< object_image<I> > data_t;
    this->data_
      = new data_t(duplicate(model.hook_data_()->ima_),
		   model.nlabels(),
		   model.hook_data_()->bboxes_,
		   model.hook_data_()->mass_centers_);
  }


  template <typename I>
  inline
  const util::array<typename object_image<I>::ms_t>&
  object_image<I>::mass_centers() const
  {
    return this->data_->mass_centers_;
  }

  template <typename I>
  inline
  const mln_psite(I)&
  object_image<I>::mass_center(const mln_value(I)& label) const
  {
    return this->data_->mass_centers_(label);
  }


  template <typename I>
  inline
  const I&
  object_image<I>::labeled_image_() const
  {
    return this->data_->ima_;
  }

  template <typename I>
  inline
  void
  object_image<I>::init_update_data_()
  {
    // this->nlabels_ is supposed to be updated when this method is
    // called.
    this->data_->tmp_accus_.resize(static_cast<unsigned>(this->data_->nlabels_) + 1);
  }


  template <typename I>
  inline
  void
  object_image<I>::prepare_update_data_(const mln_value(I)& lbl,
					const mln_value(I)& new_lbl)
  {
    this->data_->tmp_accus_[new_lbl].take(this->data_->mass_centers_[lbl]);
  }


  template <typename I>
  inline
  void
  object_image<I>::update_data_(
    const fun::i2v::array<mln_value(I)>& relabel_fun)
  {
    (void) relabel_fun;
    convert::from_to(this->data_->tmp_accus_, this->data_->mass_centers_);
  }


  // init_

  template <typename I, typename J>
  void init_(tag::image_t, object_image<I>& target,
	     const object_image<J>& model)
  {
    I ima;
    init_(tag::image, ima, model);
    target.init_(ima, model.nlabels());
  }


  // Make routines.

  namespace make
  {

    template <typename I>
    mln::object_image<I>
    object_image(const Image<I>& ima, const mln_value(I)& nlabels)
    {
      mln_precondition(exact(ima).is_valid());
      mln::object_image<I> tmp(exact(ima), nlabels);
      return tmp;
    }

  } // end of namespace mln::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# define object_image(L) mln::object_image<L>

#endif // ! SCRIBO_CORE_OBJECT_IMAGE_HH
