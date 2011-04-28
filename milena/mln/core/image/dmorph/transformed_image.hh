// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_DMORPH_TRANSFORMED_IMAGE_HH
# define MLN_CORE_IMAGE_DMORPH_TRANSFORMED_IMAGE_HH

/// \file
///
/// Definition of an image morpher that has a site set transformed
/// through a function.
///
/// \todo It is a rough code.

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/site_set/p_transformed.hh>


namespace mln
{


  // Forward declaration.
  template <typename I, typename F> struct transformed_image;


  namespace internal
  {

    /// Data structure for \c mln::transformed_image<I,F>.
    template <typename I, typename F>
    struct data< transformed_image<I,F> >
    {
      data(I& ima, const F& f);

      I ima_;
      F f_;
      p_transformed< mln_domain(I), F> domain_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename F>
    struct image_< transformed_image<I,F> > : default_image_morpher< I,
								     mln_value(I),
								     transformed_image<I,F> >
    {
      typedef trait::image::category::domain_morpher category;

      typedef trait::image::ext_domain::none      ext_domain;  // No extension of domain.
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_alignment::not_aligned   value_alignment;
      typedef trait::image::value_storage::disrupted value_storage;
    };

  } // end of namespace mln::trait



  /// Image having its domain restricted by a site set.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename F>
  struct transformed_image : public internal::image_domain_morpher< I,
								    p_transformed< mln_domain(I), F>,
								    transformed_image<I,F> >
  {
    /// Skeleton.
    typedef transformed_image< tag::image_<I>, tag::function_<F> > skeleton;

    /// Constructor without argument.
    transformed_image();

    /// Constructor.
    transformed_image(I& ima, const F& f);

    /// Initialization.
    void init_(I& ima, const F& f);


    /// Give the definition domain.
    const p_transformed< mln_domain(I), F>& domain() const;


    /// Read-only access of pixel value at point site \p p.
    mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    /// Read and "write if possible" access of pixel value at point
    /// site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);


    /// Const promotion via conversion.
    operator transformed_image<const I, F>() const;
  };



  // Morpher creation.

  template <typename I, typename F>
  transformed_image<const I, F>
  transform_domain(const Image<I>& ima, const Function_v2v<F>& f);

  template <typename I, typename F>
  transformed_image<I, F>
  transform_domain(Image<I>& ima, const Function_v2v<F>& f);




# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename I, typename F, typename J>
  inline
  void init_(tag::image_t, transformed_image<I,F>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, exact(model));
    F f;
    init_(tag::function, f, exact(model));
    target.init_(ima, f);
  }

  template <typename I, typename F>
  void init_(tag::function_t, F& f, const transformed_image<I,F>& model)
  {
    f = model.domain().function();
  }


  // internal::data< transformed_image<I,F> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< transformed_image<I,F> >::data(I& ima, const F& f)
      : ima_(ima),
	f_(f),
	domain_(ima.domain(), f)
    {
    }

  } // end of namespace mln::internal


  // transformed_image<I,F>

  template <typename I, typename F>
  inline
  transformed_image<I,F>::transformed_image()
  {
  }

  template <typename I, typename F>
  inline
  transformed_image<I,F>::transformed_image(I& ima, const F& f)
  {
    init_(ima, f);
  }

  template <typename I, typename F>
  inline
  void
  transformed_image<I,F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< transformed_image<I,F> >(ima, f);
  }

  template <typename I, typename F>
  inline
  const p_transformed< mln_domain(I), F>&
  transformed_image<I,F>::domain() const
  {
    return this->data_->domain_;
  }

  template <typename I, typename F>
  inline
  mln_rvalue(I)
  transformed_image<I,F>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->delegatee_() != 0);
    mln_precondition(exact(this)->has(p));
    mln_precondition(this->delegatee_()->has(this->data_->f_(p)));
    return this->delegatee_()->operator()(this->data_->f_(p));
  }

  template <typename I, typename F>
  inline
  mln_morpher_lvalue(I)
  transformed_image<I,F>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->delegatee_() != 0);
    mln_precondition(exact(this)->has(p));
    mln_precondition(this->delegatee_()->has(this->data_->f_(p)));
    return this->delegatee_()->operator()(this->data_->f_(p));
  }


  template <typename I, typename F>
  inline
  transformed_image<I,F>::operator transformed_image<const I, F>() const
  {
    transformed_image<const I, F> tmp(this->data_->ima_,
				      this->data_->f_);
    return tmp;
  }


  // Morpher creation.

  template <typename I, typename F>
  inline
  transformed_image<const I, F>
  transform_domain(const Image<I>& ima, const Function_v2v<F>& f)
  {
    transformed_image<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  inline
  transformed_image<I, F>
  transform_domain(Image<I>& ima, const Function_v2v<F>& f)
  {
    transformed_image<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_TRANSFORMED_IMAGE_HH
