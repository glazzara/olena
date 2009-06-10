// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_STACK_HH
# define MLN_VALUE_STACK_HH

/// \file
///
/// Definition of the stack image type.

# include <mln/core/internal/image_value_morpher.hh>

# include <mln/algebra/vec.hh>
# include <mln/value/set.hh>
# include <mln/value/proxy.hh>


namespace mln
{

  // Forward declaration.
  namespace value { template <unsigned n, typename I> struct stack_image; }

  namespace internal
  {


    /// data structure for stack_image.
    ///
    template <unsigned n, typename I>
    struct data< value::stack_image<n, I> >
    {
    public:
      data(const algebra::vec<n,I>& imas);
      algebra::vec<n,I> imas_;
      I& ima_;
    };

  }

  namespace value
  {

    namespace internal
    {
      template <unsigned n, typename I>
      struct helper_stack_image_lvalue_
      {
	typedef value::proxy< stack_image<n,I> > ret;
	static ret make(stack_image<n,I>& ima, const mln_psite(I)& p)
	{
	  ret tmp(ima, p);
	  return tmp;
	}
      };

      template <unsigned n, typename I>
      struct helper_stack_image_lvalue_< n, const I >
      {
	typedef algebra::vec<n, mln_value(I)> ret;
	static ret make(stack_image<n, const I>& ima, const mln_psite(I)& p)
	{
	  return ima.read_(p);
	}
      };


    } // end of namespace mln::value::internal

  } // end of namespace mln::value


  namespace trait
  {

    template <unsigned n, typename I>
    struct image_< mln::value::stack_image<n, I> >
      : default_image_morpher< I,
				algebra::vec<n, mln_value(I)>,
				mln::value::stack_image<n, I> >
    {
      // FIXME: We shall carefully define the missing required traits
      // here.
      typedef trait::image::category::value_morpher category;

      typedef trait::image::nature::vectorial nature;
      /* FIXME: Setting the speed trait of a stack_image to `fast' is
         a bad approximation.

	 The value cannot reasonnably be `fastest', as several images
         in different memory locations are involved in a stack image.
         So we cannot just rely on the speed trait of I.
         Nevertheless, we cannot guarantee that a stack_image will be
         a least `fast': think of a stack_image over a ``slow''
         image_type (e.g., an image whose values are computed).  That
         image would retain the `slow' value of the speed trait.

         In conclusion, this value of the speed trait should be
         computed too.  */
      typedef trait::image::speed::fast      speed;
    };

  } // end of namespace mln::trait


  namespace value
  {
    /// Stack image class.
    ///
    /// mln::value::stack_image stores a vector of n images of the same
    /// domain.
    ///
    /// The parameter \c n is the number of images, \c I is the type of
    /// a stack element. Acces a value will compute a vector which
    /// contains n coordinates :
    ///                              [stack[0](p),
    ///                               stack[1](p),
    ///                               ... ,
    ///                               stack[n](p)]
    ///
    template <unsigned n, typename I>
    struct stack_image
      : public mln::internal::image_value_morpher< I,
                                                   algebra::vec<n, mln_value(I)>,
                                                   stack_image<n,I> >
    {
      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Site_Set associated type.
      typedef mln_domain(I) domain_t;

      /// Value associated type.
      typedef algebra::vec<n, mln_value(I)> value;

      /// Return type of read-only access.
      ///
      /// The rvalue type is not a const reference, since the value
      /// type is built on the fly, and return by value (copy).
      typedef value rvalue;

      /// Return type of read-write access.
      typedef typename internal::helper_stack_image_lvalue_<n,I>::ret lvalue;


      /// Skeleton.
      typedef stack_image< n, tag::image_<I> > skeleton;


      /// Constructors.
      /// \{
      stack_image(const algebra::vec<n,I>& imas);
      stack_image();
      /// \}

      /// Initialize an empty image.
      void init_(const algebra::vec<n,I>& imas);

      /// Test if this image has been initialized.
      bool is_valid() const;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const psite& p) const;
      rvalue read_(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const psite&);
      void write_(const psite& p, const value& v);
    };

    /// \{ Shortcut to build a stack with two images.
    template <typename I>
    stack_image<2, const I>
    stack(const Image<I>& ima1, const Image<I>& ima2);


    template <typename I>
    stack_image<2, I>
    stack(Image<I>& ima1, Image<I>& ima2);
    /// \}

  } // end of namespace mln::value

# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {
    // internal::data< cast_image_<T,I> >

    template <unsigned n, typename I>
    inline
    data< value::stack_image<n,I> >::data(const algebra::vec<n,I>& imas)
      : imas_(imas),
	ima_(imas_[0])
    {
    }

  } // end of namespace mln::internal


  namespace value
  {
    // stack_image<n, I>

    template <unsigned n, typename I>
    inline
    stack_image<n,I>::stack_image()
    {
    }

    template <unsigned n, typename I>
    inline
    stack_image<n,I>::stack_image(const algebra::vec<n,I>& imas)
    {
      init_(imas);
    }

    template <unsigned n, typename I>
    inline
    void
    stack_image<n,I>::init_(const algebra::vec<n,I>& imas)
    {
      this->data_ = new mln::internal::data< stack_image<n, I> >(imas);
      for (unsigned i = 0; i < n; ++i)
      {
	mln_precondition(imas[i].is_valid());
      }
    }

    template <unsigned n, typename I>
    inline
    bool stack_image<n,I>::is_valid() const
    {
      for (unsigned i = 0; i < n; ++i)
	mln_invariant(this->data_->imas_[i].is_valid());
      return true;
    }

    template <unsigned n, typename I>
    inline
    typename stack_image<n,I>::rvalue
    stack_image<n,I>::read_(const psite& p) const
    {
      mln_precondition(this->has(p));
      algebra::vec<n, mln_value(I)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = this->data_->imas_[i].operator()(p);
      return tmp;
    }

    template <unsigned n, typename I>
    inline
    typename stack_image<n,I>::rvalue
    stack_image<n,I>::operator()(const psite& p) const
    {
      return read_(p);
    }

    template <unsigned n, typename I>
    inline
    void
    stack_image<n,I>::write_(const psite& p, const value& v)
    {
      mln_precondition(this->has(p));
      // FIXME!!!
      for (unsigned i = 0; i < n; ++i)
	this->data_->imas_[i].operator()(p) = v[i];
    }

    template <unsigned n, typename I>
    inline
    typename stack_image<n,I>::lvalue
    stack_image<n,I>::operator()(const psite& p)
    {
      return internal::helper_stack_image_lvalue_<n,I>::make(*this, p);
    }

    // stack(..)

    template <typename I>
    inline
    stack_image<2, const I>
    stack(const Image<I>& ima1, const Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      algebra::vec<2, const I> imas;
      imas[0] = exact(ima1);
      imas[1] = exact(ima2);
      return imas;
    }

    template <typename I>
    inline
    stack_image<2, I>
    stack(Image<I>& ima1, Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      algebra::vec<2, I> imas;
      imas[0] = exact(ima1);
      imas[1] = exact(ima2);
      return imas;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_STACK_HH
