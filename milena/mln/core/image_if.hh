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

#ifndef MLN_CORE_IMAGE_IF_HH
# define MLN_CORE_IMAGE_IF_HH

/*! \file mln/core/image_if.hh
 *
 * \brief Definition of a image_if image.
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/pset_if.hh>
# include <mln/pw/all.hh>


namespace mln
{

  // Fwd decl.
  template <typename I, typename F> struct image_if;


  namespace internal
  {
    template <typename I, typename F>
    struct data_< image_if<I,F> >
    {
      data_(I& ima, const F& f);

      pset_if<mln_pset(I), F> pset_;
      F f_;
      I ima_;
      // FIXME for matthieu: f_ is *also* in pset_; pb: redundancy!!!
      // FIXME for matthieu: ctor arg list and attr list should be the *same*!
      // FIXME for matthieu: do *not* change order of those lists elements!
    };

  } // end of namespace mln::internal


  /*! \brief A base class for image adaptors.
   *
   */
  template <typename I, typename F>
  struct image_if
    : public internal::image_base_< pset_if<mln_pset(I), F>, image_if<I,F> >
  // FIXME for matthieu: *not* image_base_ *but* image_domain_morpher_
  {

    // Parent
    typedef internal::image_base_< pset_if<mln_pset(I), F>, image_if<I,F> > super_;
    // FIXME for matthieu: this should *not* be public!
    // FIXME for matthieu: comments end with a '.'
    // FIXME for matthieu: we do not say "parent" but "super".


    /// Skeleton.
    typedef image_if< tag::image<I>, tag::function<F> > skeleton;


    /// Point_Set associated type.
    typedef pset_if<mln_pset(I), F> pset;

    /// Constructor from an image \p ima and a predicate \p f.
    image_if(I& ima, const F& f);

    /// Constructor without argument.
    image_if();

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_psite(I)& p) const;

    /// Give the definition domain.
    const pset& domain() const;

    /// Const promotion via convertion.
    operator image_if<const I, F>() const;

    using super_::data_;
    // FIXME for matthieu: this should *not* be public!

    typedef image_if<I,F> self_;

    // FIXME : to put into an identity morpher
    // FIXME for matthieu: nope...


    // FIXME for matthieu: most of those typedefs and methods are useless...

    /// Point_Site associated type.
    typedef mln_point(I) point;
    typedef mln_psite(pset) psite;

    /// Value_Set associated type.
    typedef mln_vset(I) vset;

    /// Value associated type.
    typedef mln_value(I)   value;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    typedef typename internal::morpher_lvalue_<I>::ret lvalue;

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const psite& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const psite& p);

    /// Give the set of values.
    const vset& values() const;
  };



  // Operators.

  template <typename I, typename F>
  image_if<I, F>
  operator | (Image<I>& ima, const Function_p2b<F>& f);

  template <typename I, typename F>
  image_if<const I, F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f);

  template <typename I>
  image_if< I,
	    fun::equal_p2b_expr_< pw::value_<I>,
				  pw::cst_<mln_value(I)> > >
  operator | (Image<I>& ima, const mln_value(I)& v);

  template <typename I>
  image_if< const I,
	    fun::equal_p2b_expr_< pw::value_<I>,
				  pw::cst_<mln_value(I)> > >
  operator | (Image<I>& ima, const mln_value(I)& v);

  // FIXME: Add the notion of "interval of values"...
  // FIXME: so we can write:  ima | from_to(v1, v2)



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< image_if<I,S> >

    template <typename I, typename F>
    data_< image_if<I,F> >::data_(I& ima, const F& f)
      : ima_(ima),
	pset_(ima.domain() | f),
	f_(f)
    {
    }

  } // end of namespace mln::internal

  // image_if<I,F>

  template <typename I, typename F>
  image_if<I,F>::image_if()
  {
  }

  template <typename I, typename F>
  image_if<I,F>::image_if(I& ima, const F& f)
  {
    this->data_ = new internal::data_< image_if<I,F> >(ima, f);
  }

  template <typename I, typename F>
  bool
  image_if<I,F>::owns_(const mln_psite(I)& p) const
  {
    return data_->pset_.has(p);
  }

  template <typename I, typename F>
  const pset_if<mln_pset(I), F>&
  image_if<I,F>::domain() const
  {
    return data_->pset_;
  }

  template <typename I, typename F>
  image_if<I,F>::operator image_if<const I, F>() const
  {
    image_if<const I, F> tmp(this->data_->ima_, this->data_->f_);
    return tmp;
  }


  template <typename I, typename F>
  typename image_if<I,F>::rvalue
  image_if<I,F>::operator()(const psite& p) const
  {
    mln_precondition(exact(this)->owns_(p));
    return data_->ima_(p);
  }

  template <typename I, typename F>
  typename image_if<I,F>::lvalue
  image_if<I,F>::operator()(const psite& p)
  {
    mln_precondition(exact(this)->owns_(p));
    return data_->ima_(p);
  }

  template <typename I, typename F>
  const mln_vset(I)&
  image_if<I,F>::values() const
  {
    return data_->ima_.values();
  }


  // Operators.

  template <typename I, typename F>
  image_if<I, F>
  operator | (Image<I>& ima, const Function_p2b<F>& f)
  {
    image_if<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  image_if<const I, F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f)
  {
    image_if<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I>
  image_if< I,
	    fun::equal_p2b_expr_< pw::value_<I>,
				  pw::cst_<mln_value(I)> > >
  operator | (Image<I>& ima, const mln_value(I)& v)
  {
    return ima | (pw::value(ima) == pw::cst(v));
  }

  template <typename I>
  image_if< const I,
	    fun::equal_p2b_expr_< pw::value_<I>,
				  pw::cst_<mln_value(I)> > >
  operator | (const Image<I>& ima, const mln_value(I)& v)
  {
    return ima | (pw::value(ima) == pw::cst(v));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IF_HH
