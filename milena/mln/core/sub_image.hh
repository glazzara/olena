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

#ifndef MLN_CORE_SUB_IMAGE_HH
# define MLN_CORE_SUB_IMAGE_HH

/*!
 * \file   sub_image.hh
 *
 * \brief Definition of morpher that makes an image become restricted
 * given by a point set.
 *
 */

# include <mln/core/internal/image_domain_morpher.hh>


namespace mln
{


  // Fwd decl.
  template <typename I, typename S> class sub_image;


  namespace internal
  {

    /// \internal Data structure for \c mln::sub_image<I,S>.
    template <typename I, typename S>
    struct data_< sub_image<I,S> >
    {
      data_(I& ima, const S& pset);

      I ima_;
      S pset_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename S>
    struct image_< sub_image<I,S> > : default_image_morpher_< I, mln_value(I),
							      sub_image<I,S> >
    {
//     private:
//       typedef mln_trait_image_data(I) I_data_;
//       typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
//     public:

      typedef trait::image::category::domain_morpher category;

      typedef trait::image::border::none border;  // no more accessible border

      typedef mln_trait_image_io_from_(I) io; // un-write when I const

      typedef mln_trait_image_data_from_(I) data;
//       typedef mlc_if( I_data_are_linear_,
// 		      trait::data::stored, // if linear then just stored
// 		      I_data_ ) data;      // otherwise like I
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename I, typename S>
  struct sub_image : public internal::image_domain_morpher_< I, S, sub_image<I,S> >
  {
    /// Skeleton.
    typedef sub_image< tag::image_<I>, tag::pset_<S> > skeleton;

    /// Constructor without argument.
    sub_image();

    /// Constructor.
    sub_image(I& ima, const S& pset);

    /// Initialization.
    void init_(I& ima, const S& pset);

    /// Give the definition domain.
    const S& domain() const;

    /// Const promotion via convertion.
    operator sub_image<const I, S>() const;
  };




  template <typename I, typename S>
  sub_image<const I, S> operator|(const Image<I>& ima, const Point_Set<S>& pset);

  template <typename I, typename S>
  sub_image<I, S> operator|(Image<I>& ima, const Point_Set<S>& pset);



  template <typename I, typename S, typename J>
  void init_(tag::image_t, sub_image<I,S>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename I, typename S, typename J>
  inline
  void init_(tag::image_t, sub_image<I,S>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    S pset;
    init_(tag::domain, pset, model);
    target.init_(ima, pset);
  }


  // internal::data_< sub_image<I,S> >

  namespace internal
  {

    template <typename I, typename S>
    inline
    data_< sub_image<I,S> >::data_(I& ima, const S& pset)
      : ima_(ima),
	pset_(pset)
    {
    }

  } // end of namespace mln::internal


  // sub_image<I,S>

  template <typename I, typename S>
  inline
  sub_image<I,S>::sub_image()
  {
  }

  template <typename I, typename S>
  inline
  sub_image<I,S>::sub_image(I& ima, const S& pset)
  {
    init_(ima, pset);
  }

  template <typename I, typename S>
  inline
  void
  sub_image<I,S>::init_(I& ima, const S& pset)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< sub_image<I,S> >(ima, pset);
  }

  template <typename I, typename S>
  inline
  const S&
  sub_image<I,S>::domain() const
  {
    return this->data_->pset_;
  }

  template <typename I, typename S>
  inline
  sub_image<I,S>::operator sub_image<const I, S>() const
  {
    sub_image<const I, S> tmp(this->data_->ima_,
			      this->data_->pset_);
    return tmp;
  }


  // Operators.

  template <typename I, typename S>
  inline
  sub_image<const I, S>
  operator|(const Image<I>& ima, const Point_Set<S>& pset)
  {
    sub_image<const I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

  template <typename I, typename S>
  inline
  sub_image<I, S>
  operator|(Image<I>& ima, const Point_Set<S>& pset)
  {
    sub_image<I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SUB_IMAGE_HH
