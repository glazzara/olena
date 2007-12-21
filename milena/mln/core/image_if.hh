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
 * \brief Definition of a image which domain is restricted by a
 * function.
 *
 * \todo FIXME for theo : add image_if_great_than / less_than
 */

# include <mln/core/internal/image_if_base.hh>


# define  Super  mln::internal::image_if_base_< I, F, image_if<I,F> >


namespace mln
{

  // Fwd decl.
  template <typename I, typename F> struct image_if;


  namespace internal
  {

    /// \internal Data structure for \c mln::image_if<I,F>.
    template <typename I, typename F>
    struct data_< image_if<I,F> > : data_< Super >
    {
      data_(I& ima, const F& f);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename F>
    struct image_< image_if<I,F> > : trait::image_< Super >
    {
    };

  } // end of namespace mln::trait



  /*! \brief Image which domain is restricted by a function.
   *
   */
  template <typename I, typename F>
  struct image_if : public Super
  {
    /// Skeleton.
    typedef image_if< tag::image_<I>, tag::function_<F> > skeleton;

    /// Constructor from an image \p ima and a predicate \p f.
    image_if(I& ima, const F& f);

    /// Constructor without argument.
    image_if();

    /// Const promotion via convertion.
    operator image_if<const I, F>() const;
  };


  // init_.

  template <typename I, typename F>
  void init_(tag::function_t, F& f, const image_if<I,F>& model)
  {
    f = model.domain().predicate();
  }

  template <typename I, typename F, typename J>
  void init_(tag::image_t, image_if<I,F>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, exact(model));
    F f;
    init_(tag::function, f, exact(model));
    target.init_(ima, f);
  }


  // Operators.

  // Image | Function_p2b.

  /// ima | f creates an image_if with the image ima and the function
  /// f.
  template <typename I, typename F>
  image_if<I,F>
  operator | (Image<I>& ima, const Function_p2b<F>& f);

  /// ima | f creates an image_if with the image ima and the function
  /// f.
  template <typename I, typename F>
  image_if<const I,F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f);


# ifndef MLN_INCLUDE_ONLY

  /// \internal internal::data_

  namespace internal
  {

    template <typename I, typename F>
    inline
    data_< image_if<I,F> >::data_(I& ima, const F& f)
      : data_< Super >(ima, f)
    {
    }

  }

  // image_if<I,F>

  template <typename I, typename F>
  inline
  image_if<I,F>::image_if()
  {
  }

  template <typename I, typename F>
  inline
  image_if<I,F>::image_if(I& ima, const F& f)
  {
    this->init_(ima, f);
  }

  template <typename I, typename F>
  inline
  image_if<I,F>::operator image_if<const I,F>() const
  {
    mln_precondition(this->has_data());
    image_if<const I,F> tmp(this->data_->ima_,
			    this->data_->pset_.predicate());
    return tmp;
  }

  // Operators.

  template <typename I, typename F>
  inline
  image_if<I,F>
  operator | (Image<I>& ima, const Function_p2b<F>& f)
  {
    image_if<I,F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  inline
  image_if<const I, F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f)
  {
    image_if<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# undef Super


#endif // ! MLN_CORE_IMAGE_IF_HH
