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

#ifndef MLN_CORE_INTERNAL_IMAGE_IF_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_IF_BASE_HH

/*! \file mln/core/internal/image_if_base.hh
 *
 * \brief Definition of a image_if_base image.
 */

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/site_set/p_if.hh>
# include <mln/pw/all.hh>
# include <mln/value/interval.hh>


namespace mln
{


  namespace internal
  {

    // Fwd decl.
    template <typename I, typename F, typename E> struct image_if_base_;

    // data_.

    template <typename I, typename F, typename E>
    struct data< image_if_base_<I,F,E> >
    {
      data(I& ima, const F& f);

      I ima_;
      p_if<mln_pset(I), F> pset_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename F, typename E>
    struct image_< mln::internal::image_if_base_<I,F,E> > : default_image_morpher< I, mln_value(I), E >
    {
    private:
      typedef mln_trait_image_data(I) I_data_;
      typedef mlc_equal(I_data_, trait::image::data::linear) I_data_are_linear_;
    public:

      typedef trait::image::category::domain_morpher category;

      typedef mlc_if( mlc_is_const(I),
		      trait::image::io::read_only, // I const => read_only
		      mln_trait_image_io(I) ) io;  // otherwise like I

      typedef mlc_if( I_data_are_linear_,
		      trait::image::data::stored, // if linear then just stored
		      I_data_ ) data;             // otherwise like I

      typedef mln_trait_image_border(I) border;
    };

  } // end of namespace mln::trait



  namespace internal
  {

    /*! An base image class for image_if_'something.
     *
     */
    template <typename I, typename F, typename E>
    struct image_if_base_ : public internal::image_domain_morpher< I, p_if<mln_pset(I),F>, E >
    {

      /// Give the definition domain.
      const p_if<mln_pset(I), F>& domain() const;

      void init_(I& ima, const F& f);

      /// Test if the image owns the point site \p p.
      /// The result is the same than over the underlying image.
      bool has(const mln_psite(I)& p) const;

    protected:

      /// Constructor from an image \p ima and a predicate \p f.
      image_if_base_(I& ima, const F& f);

      /// Constructor without argument.
      image_if_base_();
    };


# ifndef MLN_INCLUDE_ONLY

//     // init_

//     template <typename I, typename F, typename E>
//     void init_(tag::function_t, F& f, const image_if_base_<I,F,E>& model)
//     {
//       f = model.domain().predicate();
//     }

//     template <typename I, typename F, typename E, typename J>
//     void init_(tag::image_t, image_if_base_<I,F,E>& target, const J& model)
//     {
//       I ima;
//       init_(tag::image, ima, exact(model));
//       F f;
//       init_(tag::function, f, exact(model));
//       target.init_(ima, f);
//     }

    // internal::data< image_if_base_<I,S> >

    template <typename I, typename F, typename E>
    inline
    data< image_if_base_<I,F,E> >::data(I& ima, const F& f)
      : ima_(ima),
	pset_(ima.domain() | f)
    {
    }

    // image_if_base_<I,F,E>

    template <typename I, typename F, typename E>
    inline
    image_if_base_<I,F,E>::image_if_base_()
    {
    }

    template <typename I, typename F, typename E>
    inline
    image_if_base_<I,F,E>::image_if_base_(I& ima, const F& f)
    {
      init_(ima, f);
    }

    template <typename I, typename F, typename E>
    inline
    bool
    image_if_base_<I,F,E>::has(const mln_psite(I)& p) const
    {
      mln_precondition(this->has_data());
      return this->data_->ima_.has(p);
    }

    template <typename I, typename F, typename E>
    inline
    void
    image_if_base_<I,F,E>::init_(I& ima, const F& f)
    {
      mln_precondition(! this->has_data());
      this->data_ = new internal::data<E>(ima, f);
    }

    template <typename I, typename F, typename E>
    inline
    const p_if<mln_pset(I), F>&
    image_if_base_<I,F,E>::domain() const
    {
      return this->data_->pset_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_IF_BASE_HH
