// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_IMAGE_VMORPH_THRU_IMAGE_HH
# define MLN_CORE_IMAGE_VMORPH_THRU_IMAGE_HH

/// \file
///
/// \brief Definition of a morpher that morph image values through a function.
///
/// \todo Debug constness of thru_image

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/core/concept/meta_function.hh>
# include <mln/metal/bexpr.hh>
# include <mln/trait/functions.hh>


namespace mln
{

  // Forward declaration.
  template <typename I, typename F> class thru_image;

  namespace internal
  {
    template <typename I, typename F> class thru_image_write;
    template <typename I, typename F> class thru_image_read;

    /*!
      \brief Find correct implementation
    */
    template <typename I, typename F>
    struct thru_find_impl
    {
      typedef thru_image_write<I, F> write;
      typedef thru_image_read<I, F> read;
      typedef mlc_if(mlc_and(mln_trait_fun_is_assignable(F),
		     mlc_and(mlc_not(mlc_is_const(I)),
			     mlc_equal(mln_trait_image_pw_io(I),
				       trait::image::pw_io::read_write))),
		     write, read) ret;
    };

    /*!
      \brief Data structure for \c mln::thru_image<I>.
    */
    template <typename I, typename F>
    struct data< thru_image<I, F> >
    {
      data(I& ima, const F& f);

      I ima_;
      F f_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename F>
    struct image_< thru_image<I, F> > : image_< typename mln::internal::thru_find_impl<I, F>::ret > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::value_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::computed value_access;
    };

    template <typename I, typename F>
    struct image_< mln::internal::thru_image_write<I, F> > : image_< I > // Same as I except...
    {
      typedef trait::image::vw_io::read_write vw_io;
    };

    template <typename I, typename F>
    struct image_< mln::internal::thru_image_read<I, F> > : image_< I > // Same as I except...
    {
      typedef trait::image::vw_io::read vw_io;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  namespace internal
  {

    template <typename I, typename F>
    class thru_image_read : public internal::image_value_morpher< I, typename F::result, thru_image<I,F> >
    {
    public:

      /// Skeleton.
      typedef thru_image<tag::image_<I>, F> skeleton;

      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Value associated type.
      typedef mln_result(F) value;

      /// Return type of read-only access.
      typedef value rvalue;
      typedef value lvalue; // Workaround for write operator()

      rvalue operator()(const mln_psite(I)& p) const;
      rvalue operator()(const mln_psite(I)& p);

    };

    // Inheritance from read ?!
    template <typename I, typename F>
    class thru_image_write : public thru_image_read<I,F>
    {
      public:

	/// Type returned by the read-write pixel value operator.
// 	typedef typename F::template lresult<typename F::argument>::ret lvalue;
	typedef typename F::lresult lvalue;

	using thru_image_read<I,F>::operator();
	lvalue operator()(const mln_psite(I)& p);

    };
  }

  /// Morph image values through a function.
  ///
  /// \ingroup modimagevaluemorpher
  //
  template <typename I, typename F>
  class thru_image : public internal::thru_find_impl<I, F>::ret
  {
  public:

    thru_image();
    thru_image(I& ima);
    thru_image(I& ima, const F& f);


    /// \cond INTERNAL_API
    void init_(I& ima, const F& f);
    /// \endcond

    /// Const promotion via conversion.
    operator thru_image<const I, F>() const;
  };

  template <typename I, typename F>
  thru_image<I, F> thru(const mln::Function<F>& f,
			Image<I>& ima);

  template <typename I, typename F>
  const thru_image<const I, F> thru(const mln::Function<F>& f,
				    const Image<I>& ima);

  template <typename I, typename M>
  thru_image<I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, Image<I>& ima);

  template <typename I, typename M>
  const thru_image<const I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, const Image<I>& ima);

# ifndef MLN_INCLUDE_ONLY

  // internal::data< thru_image<I,S> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< thru_image<I, F> >::data(I& ima, const F& f)
      : ima_(ima),
	f_(f)
    {
    }

  } // end of namespace mln::internal

  // thru_image<I>

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image()
  {
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image(I& ima, const F& f)
  {
    mln_precondition(ima.is_valid());
    init_(ima, f);
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image(I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima, F());
  }

  template <typename I, typename F>
  inline
  void
  thru_image<I, F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->is_valid());
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< thru_image<I, F> >(ima, f);
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::operator thru_image<const I, F>() const
  {
    thru_image<const I, F> tmp(this->data_->ima_, this->data_->f_);
    return tmp;
  }

  namespace internal
  {

    template <typename I, typename F>
    inline
    typename thru_image_read<I, F>::rvalue
    thru_image_read<I, F>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(this->is_valid());
      return this->data_->f_(this->data_->ima_(p));
    }

    template <typename I, typename F>
    inline
    typename thru_image_read<I, F>::rvalue
    thru_image_read<I, F>::operator()(const mln_psite(I)& p)
    {
      mln_precondition(this->is_valid());
      return this->data_->f_(this->data_->ima_(p));
    }

    template <typename I, typename F>
    inline
    typename thru_image_write<I, F>::lvalue
    thru_image_write<I, F>::operator()(const mln_psite(I)& p)
    {
      mln_precondition(this->is_valid());
      return this->data_->f_(this->data_->ima_(p));
    }

  }

  // thru
  template <typename I, typename F>
  thru_image<I, F> thru(const mln::Function<F>& f,
			Image<I>& ima)
  {
    thru_image<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  thru_image<const I, F> thru(const mln::Function<F>& f,
			      const Image<I>& ima)
  {
    thru_image<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename M>
  thru_image<I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thru_image<I, F> tmp(exact(ima), F(exact(f).state()));

    return tmp;
  }

  template <typename I, typename M>
  thru_image<const I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, const Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thru_image<const I, F> tmp(exact(ima), F(exact(f).state()));

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VMORPH_THRU_IMAGE_HH
