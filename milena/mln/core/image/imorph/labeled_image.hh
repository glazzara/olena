// Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_HH
# define MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_HH

/// \file
///
/// \brief Definition of a morpher on a labeled image.

# include <mln/core/internal/labeled_image_base.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/data/compute.hh>

# include <mln/util/array.hh>

# include <mln/accu/center.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/accu/stat/max.hh>

# include <mln/data/compute.hh>

# include <mln/labeling/compute.hh>

namespace mln
{

  // Forward declarations.
  template <typename I> class labeled_image;
  namespace accu
  {
    template <typename T> struct nil;
    template <typename T> struct bbox;
  }


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::labeled_image<I>.
    */
    template <typename I>
    struct data< labeled_image<I> >
      : data< labeled_image_base<I, labeled_image<I> > >
    {
      typedef data< labeled_image_base<I, labeled_image<I> > > super_;

      data(const I& ima, const mln_value(I)& nlabels);
      data(const I& ima, const mln_value(I)& nlabels,
	   const util::array<mln_box(I)>& bboxes);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< labeled_image<I> >
      : image_< labeled_image_base<I, labeled_image<I> > >
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
  class labeled_image
    : public labeled_image_base<I, labeled_image<I> >
  {
    typedef labeled_image_base<I, labeled_image<I> > super_;

  public:

    /// Skeleton.
    typedef labeled_image< tag::image_<I> > skeleton;

    /// Constructors
    /// @{
    /// Constructor without argument.
    labeled_image();

    /// Constructor from an image \p ima and the number of labels \p nlabels.
    labeled_image(const I& ima, const mln_value(I)& nlabels);

    /// Constructor from an image \p ima, the number of labels \p
    /// nlabels and the object bounding boxes.
    labeled_image(const I& ima, const mln_value(I)& nlabels,
		  const util::array<mln_box(I)>& bboxes);
    /// @}

    /// Deferred initialization from a labeled image \p ima and the number
    /// of labels \p nlabels.
    void init_(const I& ima, const mln_value(I)& nlabels);

    /// Duplicate the underlying image and create a new labeled_image.
    void init_from_(const labeled_image<I>& model);

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
      : super_(ima, nlabels)
    {
    }

    template <typename I>
    inline
    data< labeled_image<I> >::data(const I& ima, const mln_value(I)& nlabels,
				   const util::array<mln_box(I)>& bboxes)
      : super_(ima, nlabels, bboxes)
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
  labeled_image<I>::labeled_image(const I& ima, const mln_value(I)& nlabels,
				  const util::array<mln_box(I)>& bboxes)
  {
    mln_precondition(data::compute(accu::meta::stat::max(), ima) == nlabels);
    this->data_ = new internal::data< labeled_image<I> >(ima, nlabels, bboxes);
  }


  template <typename I>
  inline
  void
  labeled_image<I>::init_(const I& ima, const mln_value(I)& nlabels)
  {
    mln_precondition(data::compute(accu::meta::stat::max(), ima) == nlabels);
    this->data_ = new internal::data< labeled_image<I> >(ima, nlabels);
    this->data_->bboxes_ = labeling::compute(accu::meta::shape::bbox(),
					     this->data_->ima_,
					     this->data_->nlabels_);
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

  // init_

  template <typename I, typename J>
  void init_(tag::image_t, labeled_image<I>& target,
	     const labeled_image<J>& model)
  {
    I ima;
    init_(tag::image, ima, model);
    target.init_(ima, model.nlabels());
  }


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
