// Copyright (C) 2009, 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_BASE_HH
# define MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_BASE_HH

/// \file
///
/// \brief Definition of a base class for labeled image morphers.

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/internal/image_identity.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/labeling/relabel.hh>

# include <mln/util/array.hh>
# include <mln/value/next.hh>

# include <mln/pw/cst.hh>
# include <mln/pw/value.hh>

# include <mln/make/relabelfun.hh>


namespace mln
{

  // Forward declaration.
  template <typename I, typename E>
  class labeled_image_base;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::labeled_image_base<I,E>.
    */
    template <typename I, typename E>
    struct data< labeled_image_base<I,E> >
    {
      data(const I& ima, const mln_value(I)& nlabels);
      data(const I& ima, const mln_value(I)& nlabels,
	   const util::array<mln_box(I)>& bboxes);

      I ima_;
      mln_value(I) nlabels_;
      mutable util::array< box<mln_psite(I)> > bboxes_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename E>
    struct image_< labeled_image_base<I,E> > : image_< I > // Same as I except...
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



  /*!
    \brief Base class Morpher providing an improved interface for
    labeled image.

    \tparam I The label image type.

    This image type allows to access every site set at a given
    label.

    This image type guaranties that labels are contiguous (from 1 to
    n).
  */
  template <typename I, typename E>
  class labeled_image_base
    : public internal::image_identity< const I, mln_domain(I), E >
  {
    typedef internal::image_identity< const I, mln_domain(I), E >
	    super_;

  public:
    /// Type of the bounding component bounding boxes.
    typedef mln_result(accu::shape::bbox<mln_psite(I)>) bbox_t;

    /// \name Constructors.
    /// \{

    /// Constructor without argument.
    labeled_image_base();

    /// \}

    /// \name Relabel according to a function.
    ///
    /// Merge or delete labels according to the given function.  These
    /// methods ensure that the labeling remains contiguous.
    /// \{

    // FIXME: currently the label is kept contiguous for
    // performance reasons. Do we want to be less restrictive?
    template <typename F>
    void relabel(const Function_v2v<F>& f);

    /// Labels may be removed.  This overload make sure the labeling
    /// is still contiguous.
    template <typename F>
    void relabel(const Function_v2b<F>& f);

    /// \}

    /// Return the number of labels;
    mln_value(I) nlabels() const;

    /// Return the bounding box of the component \p label.
    const bbox_t& bbox(const mln_value(I)& label) const;

    /// Return the component bounding boxes.
    const util::array<bbox_t>& bboxes() const;

    /// Return the domain of the component with label \p label.
    p_if<mln_box(I),
	 fun::eq_v2b_expr_<pw::value_<I>, pw::cst_<mln_value(I)> > >
    subdomain(const mln_value(I)& label) const;

  protected:
    /// Update bounding boxes information.
    void update_data(const fun::i2v::array<mln_value(I)>& relabel_fun);

    template <typename F>
    void relabel_(const Function_v2v<F>& f);

    template <typename F>
    void relabel_(const Function_v2b<F>& f);

    /// \name Methods called during relabeling.
    /// \{

    void init_update_data_();
    void prepare_update_data_(const mln_value(I)& lbl,
			      const mln_value(I)& new_lbl);
    void update_data_(const fun::i2v::array<mln_value(I)>& relabel_fun);

    /// \}
  };




# ifndef MLN_INCLUDE_ONLY


  // internal::data< labeled_image_base<I,E> >

  namespace internal
  {


    // data< labeled_image_base<I,E> >

    template <typename I, typename E>
    inline
    data< labeled_image_base<I,E> >::data(const I& ima, const mln_value(I)& nlabels)
      : ima_(ima), nlabels_(nlabels)
    {
    }

    template <typename I, typename E>
    inline
    data< labeled_image_base<I,E> >::data(const I& ima, const mln_value(I)& nlabels,
				   const util::array<mln_box(I)>& bboxes)
      : ima_(ima), nlabels_(nlabels), bboxes_(bboxes)
    {
    }


  } // end of namespace mln::internal


  template <typename I, typename E>
  inline
  labeled_image_base<I,E>::labeled_image_base()
  {
  }


  template <typename I, typename E>
  template <typename F>
  inline
  void
  labeled_image_base<I,E>::relabel(const Function_v2v<F>& f_)
  {
    const F& f = exact(f_);
    mln_value(I) new_nlabels;

    fun::i2v::array<mln_value(I)>
      packed_relabel_fun = make::relabelfun(f,
					    this->data_->nlabels_,
					    new_nlabels);

    labeling::relabel_inplace(this->data_->ima_, this->data_->nlabels_,
			      packed_relabel_fun);

    this->data_->nlabels_ = new_nlabels;

    exact(this)->relabel_(f);

    /// We may have merged or deleted labels.
    update_data(packed_relabel_fun);
  }


  template <typename I, typename E>
  template <typename F>
  inline
  void
  labeled_image_base<I,E>::relabel(const Function_v2b<F>& f_)
  {
    const F& f = exact(f_);

    // Relabel the underlying image.
    typedef fun::i2v::array<mln_value(I)> fv2v_t;
    fv2v_t fv2v = make::relabelfun(f,
				   this->data_->nlabels_,
				   this->data_->nlabels_);

    labeling::relabel_inplace(this->data_->ima_, this->data_->nlabels_, fv2v);

    exact(this)->relabel_(f);

    // Then, merge or delete bounding boxes according to this relabeling.
    update_data(fv2v);
  }

  template <typename I, typename E>
  inline
  mln_value(I)
  labeled_image_base<I,E>::nlabels() const
  {
    return this->data_->nlabels_;
  }


  template <typename I, typename E>
  void
  labeled_image_base<I,E>::update_data(const fun::i2v::array<mln_value(I)>& relabel_fun)
  {
    util::array<accu::shape::bbox<mln_psite(I)> >
      new_bboxes(mln::value::next(this->data_->nlabels_));

    exact(this)->init_update_data_();

    for (unsigned i = 1; i < this->data_->bboxes_.size(); ++i)
      if (relabel_fun(i) != 0)
      {
	new_bboxes[relabel_fun(i)].take(this->data_->bboxes_[i]);
	exact(this)->prepare_update_data_(i, relabel_fun(i));
      }

    convert::from_to(new_bboxes, this->data_->bboxes_);

    mln_assertion(new_bboxes.size() == this->data_->bboxes_.size());

    exact(this)->update_data_(relabel_fun);
  }


  template <typename I, typename E>
  const typename labeled_image_base<I,E>::bbox_t&
  labeled_image_base<I,E>::bbox(const mln_value(I)& label) const
  {
    return this->data_->bboxes_[label];
  }


  template <typename I, typename E>
  const util::array<typename labeled_image_base<I,E>::bbox_t>&
  labeled_image_base<I,E>::bboxes() const
  {
    return this->data_->bboxes_;
  }


  template <typename I, typename E>
  p_if<mln_box(I),
       fun::eq_v2b_expr_<pw::value_<I>, pw::cst_<mln_value(I)> > >
  labeled_image_base<I,E>::subdomain(const mln_value(I)& label) const
  {
    return ((this->data_->ima_ | bbox(label))
	    | (pw::value(this->data_->ima_) == pw::cst(label))).domain();
  }



  template <typename I, typename E>
  template <typename F>
  void
  labeled_image_base<I,E>::relabel_(const Function_v2v<F>& f)
  {
    (void) f;
    // No-Op.
  }

  template <typename I, typename E>
  template <typename F>
  void
  labeled_image_base<I,E>::relabel_(const Function_v2b<F>& f)
  {
    (void) f;
    // No-Op.
  }

  template <typename I, typename E>
  void
  labeled_image_base<I,E>::update_data_(
    const fun::i2v::array<mln_value(I)>& relabel_fun)
  {
    (void) relabel_fun;
    // No-Op.
  }

  template <typename I, typename E>
  void
  labeled_image_base<I,E>::prepare_update_data_(const mln_value(I)& lbl,
						const mln_value(I)& new_lbl)
  {
    (void) lbl;
    (void) new_lbl;
    // No-Op.
  }

  template <typename I, typename E>
  void
  labeled_image_base<I,E>::init_update_data_()
  {
    // No-Op.
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_LABELED_IMAGE_BASE_HH
