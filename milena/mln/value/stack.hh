// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_STACK_HH
# define MLN_VALUE_STACK_HH

/*! \file mln/value/stack.hh
 *
 * \brief Definition of the stack image type.
 */

# include <mln/core/internal/image_value_morpher.hh>

# include <mln/metal/vec.hh>
# include <mln/value/set.hh>
# include <mln/value/proxy.hh>


namespace mln
{

  // Fwd decl.
  namespace value { template <unsigned n, typename I> struct stack_image; }

  namespace internal
  {


    /*! \brief data structure for stack_image.
     *
     */
    template <unsigned n, typename I>
    struct data_< value::stack_image<n, I> >
    {
    public:
      data_(const metal::vec<n,I>& imas);
      metal::vec<n,I> imas_;
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
	typedef metal::vec<n, mln_value(I)> ret;
	static ret make(stack_image<n, const I>& ima, const mln_psite(I)& p)
	{
	  return ima.read_(p);
	}
      };


    } // end of namespace mln::value::internal


    /*! \brief stack image class. stack_image stores a
     *  vector of n images of the same domain.
     *
     * The parameter \c n is the number of images, \c I is the type of
     * a stack element. Acces a value will compute a vector which
     * contains n coordinates :
     *                              [stack[0](p),
     *                               stack[1](p),
     *                               ... ,
     *                               stack[n](p)]
     */
    template <unsigned n, typename I>
    struct stack_image : public mln::internal::image_value_morpher_< I, stack_image<n,I> >
    {
      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Point_Set associated type.
      typedef mln_pset(I) pset;

      /// Value associated type.
      typedef metal::vec<n, mln_value(I)> value;

      /// Return type of read-only access.
      typedef value rvalue;

      /// Return type of read-write access.
      typedef typename internal::helper_stack_image_lvalue_<n,I>::ret lvalue;

      /// Value set associated type.
      typedef mln::value::set<value> vset;


      /// Skeleton.
      typedef stack_image< n, tag::image_<I> > skeleton;


      /// Constructor.
      stack_image(const metal::vec<n,I>& imas);
      stack_image();


      /// Test if this image has been initialized.
      bool has_data() const;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const psite& p) const;
      value read_(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const psite&);
      void write_(const psite& p, const value& v);

      /// Give the set of values of the image.
      const vset& values() const;
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
    // internal::data_< cast_image_<T,I> >

    template <unsigned n, typename I>
    data_< value::stack_image<n,I> >::data_(const metal::vec<n,I>& imas)
      : imas_(imas),
	ima_(imas_[0])
    {
    }

  } // end of namespace mln::internal


  namespace value
  {
    // stack_image<n, I>

    template <unsigned n, typename I>
    stack_image<n,I>::stack_image()
    {
    }

    template <unsigned n, typename I>
    stack_image<n,I>::stack_image(const metal::vec<n,I>& imas)
    {
      this->data_ = new mln::internal::data_< stack_image<n, I> >(imas);
      for (unsigned i = 0; i < n; ++i)
      {
	mln_precondition(imas[i].has_data());
      }
    }

    template <unsigned n, typename I>
    bool stack_image<n,I>::has_data() const
    {
      for (unsigned i = 0; i < n; ++i)
	mln_invariant(this->data_->imas_[i].has_data());
      return true;
    }

    template <unsigned n, typename I>
    metal::vec<n, mln_value(I)>
    stack_image<n,I>::read_(const psite& p) const
    {
      mln_precondition(this->owns_(p));
      metal::vec<n, mln_value(I)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = this->data_->imas_[i].operator()(p);
      return tmp;
    }

    template <unsigned n, typename I>
    metal::vec<n, mln_value(I)>
    stack_image<n,I>::operator()(const psite& p) const
    {
      return read_(p);
    }

    template <unsigned n, typename I>
    void
    stack_image<n,I>::write_(const psite& p, const value& v)
    {
      mln_precondition(this->owns_(p));
      // FIXME!!!
      for (unsigned i = 0; i < n; ++i)
	this->data_->imas_[i].operator()(p) = v[i];
    }

    template <unsigned n, typename I>
    typename stack_image<n,I>::lvalue
    stack_image<n,I>::operator()(const psite& p)
    {
      return internal::helper_stack_image_lvalue_<n,I>::make(*this, p);
    }

    template <unsigned n, typename I>
    const mln::value::set< metal::vec<n, mln_value(I)> >&
    stack_image<n,I>::values() const
    {
      return vset::the();
    }

    // stack(..)

    template <typename I>
    stack_image<2, const I>
    stack(const Image<I>& ima1, const Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      metal::vec<2, const I> imas;
      imas[0] = exact(ima1);
      imas[1] = exact(ima2);
      return imas;
    }

    template <typename I>
    stack_image<2, I>
    stack(Image<I>& ima1, Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      metal::vec<2, I> imas;
      imas[0] = exact(ima1);
      imas[1] = exact(ima2);
      return imas;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_STACK_HH
