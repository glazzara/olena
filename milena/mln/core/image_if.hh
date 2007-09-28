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

# include <mln/core/internal/image_domain_morpher.hh>
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
      data_(I& ima, const pset_if<mln_pset(I), F>& pset);

      I ima_;
      pset_if<mln_pset(I), F> pset_;
    };

  } // end of namespace mln::internal


  /*! \brief A base class for image adaptors.
   *
   */
  template <typename I, typename F>
  struct image_if : public internal::image_domain_morpher_< I,
							    pset_if<mln_pset(I),F>,
							    image_if<I,F> >
  {
    /// Skeleton.
    typedef image_if< tag::image_<I>, tag::function_<F> > skeleton;

    /// Constructor from an image \p ima and a predicate \p f.
    image_if(I& ima, const F& f);

    /// Constructor without argument.
    image_if();

    /// Initialization.
    void init_(I& ima, const F& f);

    /// Initialization.
    void init_(I& ima, const pset_if<mln_pset(I), F>& pset);

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_psite(I)& p) const;

    /// Give the definition domain.
    const pset_if<mln_pset(I), F>& domain() const;

    /// Const promotion via convertion.
    operator image_if<const I, F>() const;
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
  operator | (const Image<I>& ima, const mln_value(I)& v);

  // FIXME: Add the notion of "interval of values"...
  // FIXME: so we can write:  ima | from_to(v1, v2)



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename I, typename F>
  void init_(tag::function_t, F& f, const image_if<I,F>& model)
  {
    f = model.domain().predicate();
  }

  template <typename I, typename F, typename J>
  void init_(tag::image_t, image_if<I,F>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    F f;
    init_(tag::function, f, model);
    target.init_(ima, f);
    // Alternative code:
    //   pset_if<mln_pset(I), F> pset;
    //   init_(tag::domain, pset, model);
    //   target.init_(ima, pset);
  }

  // internal::data_< image_if<I,S> >

  namespace internal
  {

    template <typename I, typename F>
    data_< image_if<I,F> >::data_(I& ima, const F& f)
      : ima_(ima),
	pset_(ima.domain() | f)
    {
    }

    template <typename I, typename F>
    data_< image_if<I,F> >::data_(I& ima, const pset_if<mln_pset(I), F>& pset)
      : ima_(ima),
	pset_(pset)
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
    init_(ima, f);
  }

  template <typename I, typename F>
  void
  image_if<I,F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image_if<I,F> >(ima, f);
  }

  template <typename I, typename F>
  void
  image_if<I,F>::init_(I& ima, const pset_if<mln_pset(I), F>& pset)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image_if<I,F> >(ima, pset);
  }

  template <typename I, typename F>
  image_if<I,F>::operator image_if<const I, F>() const
  {
    image_if<const I, F> tmp(this->data_->ima_, this->data_->pset_);
    return tmp;
  }

  template <typename I, typename F>
  bool
  image_if<I,F>::owns_(const mln_psite(I)& p) const
  {
    return this->data_->pset_.has(p);
  }

  template <typename I, typename F>
  const pset_if<mln_pset(I), F>&
  image_if<I,F>::domain() const
  {
    return this->data_->pset_;
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
