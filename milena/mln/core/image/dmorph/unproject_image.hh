// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_DMORPH_UNPROJECT_IMAGE_HH
# define MLN_CORE_IMAGE_DMORPH_UNPROJECT_IMAGE_HH

/// \file
///
/// Definition of a morpher that un-projects an image.
/// FIXME: Doc!

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/grids.hh>
# include <mln/core/site_set/box.hh>


namespace mln
{


  // Forward declaration.
  template <typename I, typename D, typename F> struct unproject_image;


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::unproject_image<I,D,F>.
    */
    template <typename I, typename D, typename F>
    struct data< unproject_image<I,D,F> >
    {
      data(I& ima, const D& dom, const F& f);

      I ima_;
      D dom_;
      F f_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename D, typename F>
    struct image_< unproject_image<I,D,F> > : default_image_morpher< I,
								     mln_value(I),
								     unproject_image<I,D,F> >
    {
      typedef trait::image::category::domain_morpher category;

      // FIXME:
//       typedef trait::image::dimension::two_d      dimension;

      typedef trait::image::ext_domain::none      ext_domain;  // No extension of domain.
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_alignment::not_aligned   value_alignment;
      typedef trait::image::value_storage::disrupted       value_storage;
    };

  } // end of namespace mln::trait



  /// Un-projects an image.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename D, typename F>
  struct unproject_image : public internal::image_domain_morpher< I,
								  D,
								  unproject_image<I,D,F> >
  {
    // FIXME:
    typedef void skeleton;
//     /// Skeleton.
//     typedef unproject_image< tag::image_<I> > skeleton;


    /// Constructor without argument.
    unproject_image();

    /// Constructor from an image \p ima, a domain \p dom, and a function \p f.
    unproject_image(I& ima, const D& dom, const F& f);

    void init_(I& ima, const D& dom, const F& f);


    /// Give the definition domain.
    const D& domain() const;

    /// Read-only access to the image value located at point \p p.
    mln_rvalue(I) operator()(const mln_psite(D)& p) const;

    /// Read-write access to the image value located at point \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(D)& p);


//     /// Const promotion via conversion.
//     operator unproject_image<const I, D, F>() const;
  };



  template <typename I, typename D, typename F>
  unproject_image<I, D, F>
  unproject(Image<I>& ima, const Site_Set<D>& dom, const Function_v2v<F>& f);

  template <typename I, typename D, typename F>
  unproject_image<const I, D, F>
  unproject(const Image<I>& ima, const Site_Set<D>& dom, const Function_v2v<F>& f);




//   namespace trait
//   {

//     template <typename I, typename D, typename F,
// 	      typename V>
//     struct ch_value< unproject_image<I,D,F>, V >
//     {
//       // FIXME: Somthing like:
//       typedef pw::image< pw::cst_<mln_value(I)>, D> I_;
//       typedef mln_ch_value(I_,V) ret;
//     };

//   } // end of namespace mln::trait



# ifndef MLN_INCLUDE_ONLY


  // internal::data< unproject_image<I,D,F> >

  namespace internal
  {

    template <typename I, typename D, typename F>
    inline
    data< unproject_image<I,D,F> >::data(I& ima, const D& dom, const F& f)
      : ima_(ima),
	dom_(dom),
	f_(f)
    {
    }

  }


  // unproject_image<I,D,F>

  template <typename I, typename D, typename F>
  inline
  unproject_image<I,D,F>::unproject_image()
  {
  }

  template <typename I, typename D, typename F>
  inline
  unproject_image<I,D,F>::unproject_image(I& ima, const D& dom, const F& f)
  {
    init_(ima, dom, f);
  }

  template <typename I, typename D, typename F>
  inline
  void
  unproject_image<I,D,F>::init_(I& ima, const D& dom, const F& f)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< unproject_image<I,D,F> >(ima, dom, f);
  }

  template <typename I, typename D, typename F>
  inline
  const D&
  unproject_image<I,D,F>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->dom_;
  }

  template <typename I, typename D, typename F>
  inline
  mln_rvalue(I)
  unproject_image<I,D,F>::operator()(const mln_psite(D)& p) const
  {
    mln_precondition(this->has(p));
    mln_result(F) p_ = this->data_->f_(p);
    mln_precondition(this->data_->ima_.has(p_));
    return this->data_->ima_(p_);
  }

  template <typename I, typename D, typename F>
  inline
  mln_morpher_lvalue(I)
  unproject_image<I,D,F>::operator()(const mln_psite(D)& p)
  {
    mln_precondition(this->has(p));
    mln_result(F) p_ = this->data_->f_(p);
    mln_precondition(this->data_->ima_.has(p_));
    return this->data_->ima_(p_);
  }


  // Routines.

  template <typename I, typename D, typename F>
  inline
  unproject_image<I, D, F>
  unproject(Image<I>& ima_, const Site_Set<D>& dom, const Function_v2v<F>& f)
  {
    mlc_converts_to(mln_psite(D), mln_argument(F))::check();
    mlc_converts_to(mln_result(F), mln_psite(I))::check();

    I& ima = exact(ima_);
    mln_precondition(ima.is_valid());

    unproject_image<I,D,F> tmp(ima, exact(dom), exact(f));
    return tmp;
  }

  template <typename I, typename D, typename F>
  inline
  unproject_image<const I, D, F>
  unproject(const Image<I>& ima_, const Site_Set<D>& dom, const Function_v2v<F>& f)
  {
    mlc_converts_to(mln_psite(D), mln_argument(F))::check();
    mlc_converts_to(mln_result(F), mln_psite(I))::check();

    const I& ima = exact(ima_);
    mln_precondition(ima.is_valid());

    unproject_image<const I,D,F> tmp(ima, exact(dom), exact(f));
    return tmp;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_IMAGE_DMORPH_UNPROJECT_IMAGE_HH
