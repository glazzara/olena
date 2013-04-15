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

#ifndef MLN_CORE_IMAGE_DMORPH_EXTENSION_FUN_HH
# define MLN_CORE_IMAGE_DMORPH_EXTENSION_FUN_HH

/// \file
///
/// definition of a morpher that extends the domain of an image
/// with a function.
///
/// \todo Deal with two-ways functions...
/// \todo Use an envelop as lvalue to test extension writing.

# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Forward declaration.
  template <typename I, typename F> class extension_fun;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::extension_fun<I, F>.
    */
    template <typename I, typename F>
    struct data< extension_fun<I, F> >
    {
      data(I& ima, const F& fun);

      I ima_;
      const F fun_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename F>
    struct image_< extension_fun<I, F> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::indirect value_access;

      // extended domain
      typedef trait::image::ext_domain::infinite ext_domain;
      typedef trait::image::ext_value::multiple  ext_value;
      typedef trait::image::ext_io::read_only    ext_io;
    };

    template <typename I, typename F, typename V>
    struct ch_value< extension_fun<I, F>, V >
    {
      typedef mlc_converts_to(mln_result(F), V) keep_ext;
      typedef mln_ch_value(I, V)   Iv;
      typedef extension_fun<Iv, F> Iv_ext;
      typedef mlc_if(keep_ext, Iv_ext, Iv) ret;
    };

  } // end of namespace mln::trait



  /// Extends the domain of an image with a function.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename F>
  class extension_fun
  : public internal::image_identity< I, mln_domain(I), extension_fun<I, F> >,
    private mlc_converts_to(mln_result(F), mln_value(I))::check_t
  {
  public:

    /// Skeleton.
    typedef extension_fun< tag::image_<I>, tag::function_<F> > skeleton;

    /// Image value type.
    typedef mln_value(I) value;

    /// Return type of read-only access.
    typedef mln_value(I) rvalue;


    /// Constructor without argument.
    extension_fun();

    /// Constructor from an image \p ima and a function \p fun.
    extension_fun(I& ima, const F& fun);

    /// \cond INTERNAL_API
    /// Deferred initialization from an image \p ima and a function \p
    /// fun.
    void init_(I& ima, const F& fun);
    /// \endcond


    /// Test if \p p is valid.  It returns always true, assuming that
    /// the function is valid for any \p p.
    // Tech note: the 'template' allows for multiple input.
    template <typename P>
    bool has(const P& p) const;


    /// Read-only access to the image value located at site \p p;
    mln_value(I) operator()(const mln_psite(I)& p) const;

    /// Read-write access to the image value located at site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);


    /// Give the extension function.
    const F& extension() const;
  };


  /// \cond INTERNAL_API

  // init_

  template <typename I, typename F, typename J>
  void init_(tag::image_t, extension_fun<I,F>& target, const J& model);

  template <typename F, typename I>
  void init_(tag::extension_t, F& target, const extension_fun<I,F>& model);

  /// \endcond


# ifndef MLN_INCLUDE_ONLY

  // internal::data< extension_fun<I,S> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< extension_fun<I, F> >::data(I& ima, const F& fun)
      : ima_(ima),
	fun_(fun)
    {
    }

  } // end of namespace mln::internal


  // extension_fun<I, F>

  template <typename I, typename F>
  inline
  extension_fun<I, F>::extension_fun()
  {
  }

  template <typename I, typename F>
  inline
  extension_fun<I, F>::extension_fun(I& ima, const F& fun)
  {
    init_(ima, fun);
  }

  template <typename I, typename F>
  inline
  void
  extension_fun<I, F>::init_(I& ima, const F& fun)
  {
    this->data_ = new internal::data< extension_fun<I, F> >(ima, fun);
  }

  template <typename I, typename F>
  template <typename P>
  inline
  bool
  extension_fun<I, F>::has(const P&) const
  {
    return true;
  }

  template <typename I, typename F>
  inline
  mln_value(I)
  extension_fun<I, F>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    // if-else is preferred to the ternary op to allow for the
    // function result to convert towards the expected return type.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      return this->data_->fun_(p);
  }

  template <typename I, typename F>
  inline
  mln_morpher_lvalue(I)
  extension_fun<I, F>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) cpy;
    mln_precondition(this->is_valid());
    // See the above comment about if-else v. ternary.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      {
	// This hack makes this signature valid both in the image
	// domain and in its extension.  It works even if
	// mln_morpher_lvalue(I) is a mutable reference.
	cpy = this->data_->fun_(p);
	return cpy;
      }
  }

  template <typename I, typename F>
  inline
  const F&
  extension_fun<I, F>::extension() const
  {
    mln_precondition(this->is_valid());
    return this->data_->fun_;
  }


  // init_

  template <typename I, typename F, typename J>
  void init_(tag::image_t, extension_fun<I,F>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    F fun;
    init_(tag::extension, fun, model);
    target.init_(ima, fun);
  }

  template <typename F, typename I>
  void init_(tag::extension_t, F& target, const extension_fun<I,F>& model)
  {
    target = model.extension();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_EXTENSION_FUN_HH
