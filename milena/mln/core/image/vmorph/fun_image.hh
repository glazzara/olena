// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_VMORPH_FUN_IMAGE_HH
# define MLN_CORE_IMAGE_VMORPH_FUN_IMAGE_HH

/// \file
///
/// Definition of an image morpher that returns image data through a
/// function.

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/trait/images.hh>
# include <mln/value/set.hh>

# include <mln/metal/if.hh>
# include <mln/metal/equal.hh>


namespace mln
{

  // Forward declaration.
  template <typename F, typename I> struct fun_image;

  namespace internal
  {
    /// Data structure for \c mln::fun_image<T,I>.
    template <typename F, typename I>
    struct data< fun_image<F,I> >
    {
      data(const F& f, const I& ima);
      F f_;
      const I ima_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename F, typename I>
    struct image_< fun_image<F,I> > : default_image_morpher< I, mln_result(F), fun_image<F,I> >
    {
      //      typedef trait::image::category::value_morpher   category;
      typedef trait::image::category::primary   category; // does not work either
      typedef trait::image::value_io::read_only      value_io;
      typedef trait::image::value_access::computed    value_access;
      typedef trait::image::value_storage::disrupted  value_storage;
      typedef
      mlc_if(mlc_equal(typename value_<mln_result(F)>::quant, trait::value::quant::low),
	// Then
	trait::image::quant::low,
	// Else
	     mlc_if(mlc_equal(typename value_<mln_result(F)>::quant, trait::value::quant::high),
		    // Then
		    trait::image::quant::high,
		    // Else
		    undef)
	    )
	quant;
    };

  } // end of namespace mln::trait




  /// Image read through a function.
  ///
  /// \ingroup modimagevaluemorpher
  //
  template <typename F, typename I>
  struct fun_image :
    public internal::image_value_morpher< I, mln_result(F), fun_image<F,I> >
  {
    /// Value associated type.
    typedef mln_result(F) value;

    /// Return type of read-only access.
    typedef mln_result(F) rvalue;

    /// Return type of read-write access.
    typedef mln_result(F) lvalue;
    // typedef mln::value::shell<F,I> lvalue;  De-activated for 1.0

    /// Skeleton.
    typedef fun_image< tag::value_<mln_result(F)>, tag::image_<I> > skeleton;

    /// Constructor.
    fun_image();

    /// Constructor.
    fun_image(const Function_v2v<F>& f, const Image<I>& ima);

    /// Constructor.
    fun_image(const Image<I>& ima);

    /// Initialize an empty image.
    void init_(const Function_v2v<F>& f, const Image<I>& ima);

    /// Read-only access of pixel value at point site \p p.
    mln_result(F) operator()(const mln_psite(I)& p) const;

    /// Mutable access is for reading only.
    mln_result(F) operator()(const mln_psite(I)& p);
    // lvalue operator()(const mln_psite(I)& p);  De-activated for 1.0
  };



  template <typename F, typename I>
  fun_image<F, I>
  operator << (const Function_v2v<F>& f, const Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY


  template <typename F, typename I>
  inline
  fun_image<F, I>
  operator << (const Function_v2v<F>& f, const Image<I>& ima)
  {
    fun_image<F, I> tmp(exact(f), exact(ima));
    return tmp;
  }


  // internal::data< fun_image<T,I> >

  namespace internal
  {

    template <typename F, typename I>
    inline
    data< fun_image<F,I> >::data(const F& f, const I& ima)
      : f_(f),
	ima_(ima)
    {
    }

  } // end of namespace mln::internal

  // fun_image<F,I>

  template <typename F, typename I>
  inline
  fun_image<F,I>::fun_image()
  {
    this->data_ = 0;
  }

  template <typename F, typename I>
  inline
  fun_image<F,I>::fun_image(const Function_v2v<F>& f, const Image<I>& ima)
  {
    init_(f, ima);
  }

  template <typename F, typename I>
  inline
  fun_image<F,I>::fun_image(const Image<I>& ima)
  {
    F f;
    init_(f, ima);
  }

  template <typename F, typename I>
  inline
  void
  fun_image<F,I>::init_(const Function_v2v<F>& f, const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data<fun_image<F,I> >(exact(f), exact(ima));
  }

  template <typename F, typename I>
  inline
  mln_result(F)
  fun_image<F,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->data_->ima_.has(p));
    return this->data_->f_( this->data_->ima_(p) );
  }

  template <typename F, typename I>
  inline
  mln_result(F)
  fun_image<F,I>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->data_->ima_.has(p));
    return this->data_->f_( this->data_->ima_(p) );
//     return mln::value::shell<F, I>( this->data_->ima_, p );  De-activated for 1.0
  }


  //  template <typename F, typename I>
  //  fun_image<F, I> fun_image(Function<F> &f, Image<I> &i)
  //  {
  //    return fun_image<F, I> (f, i);
  //  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VMORPH_FUN_IMAGE_HH
