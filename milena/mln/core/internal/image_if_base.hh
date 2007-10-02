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

#ifndef MLN_CORE_IMAGE_IF_BASE_HH
# define MLN_CORE_IMAGE_IF_BASE_HH

/*! \file mln/core/internal/image_if_base.hh
 *
 * \brief Definition of a image_if_base image.
 */

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/pset_if.hh>
# include <mln/pw/all.hh>
# include <mln/value/interval.hh>


namespace mln
{

  namespace internal
  {


    // Fwd decl.
    template <typename I, typename F> struct image_if_base;

    template <typename I, typename F>
    struct data_< image_if_base<I,F> >
    {
      data_(I& ima, const F& f);
      data_(I& ima, const pset_if<mln_pset(I), F>& pset);
	
      I ima_;
      pset_if<mln_pset(I), F> pset_;
    };
  } // end of namespace mln::internal
  
  namespace trait
  {

    template <typename I, typename F>
    struct image_< image_if_base<I,F> > : default_image_morpher_< I, mln_value(I),
								  image_if_base<I,F> >
    {
    private:
      typedef mln_trait_image_data(I) I_data_;
      typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
    public:

      typedef trait::category::domain_morpher category;

      typedef mlc_if( mlc_is_const(I),
		      trait::io::read_only,        // I const => read_only
		      mln_trait_image_io(I) ) io;  // otherwise like I

      typedef mlc_if( I_data_are_linear_,
		      trait::data::stored, // if linear then just stored
		      I_data_ ) data;      // otherwise like I
    };
    
  } // end of namespace mln::trait
  
  namespace internal
  {
    
    /*! \brief An base image class for image_if FIXME.
     *
     */
    template <typename I, typename F>
    struct image_if_base : public internal::image_domain_morpher_< I,
								   pset_if<mln_pset(I),F>,
								   image_if_base<I,F> >
    {
      /// Skeleton.
      typedef image_if_base< tag::image_<I>, F > skeleton;

      /// Constructor from an image \p ima and a predicate \p f.
      image_if_base(I& ima, const F& f);
      
      /// Constructor without argument.
      image_if_base();
      
      /// Initialization.
      void init_(I& ima, const F& f);
      
      /// Initialization.
      void init_(I& ima, const pset_if<mln_pset(I), F>& pset);

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const mln_psite(I)& p) const;
      
      /// Give the definition domain.
      const pset_if<mln_pset(I), F>& domain() const;
    };
    
    
# ifndef MLN_INCLUDE_ONLY
    
    // init_
    
    template <typename I, typename F>
    void init_(tag::function_t, F& f, const image_if_base<I,F>& model)
    {
      f = model.domain().predicate();
    }
    
    template <typename I, typename F, typename J>
    void init_(tag::image_t, image_if_base<I,F>& target, const J& model)
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
    
    // internal::data_< image_if_base<I,S> >
    
    template <typename I, typename F>
    data_< image_if_base<I,F> >::data_(I& ima, const F& f)
      : ima_(ima),
	pset_(ima.domain() | f)
    {
    }
    
    template <typename I, typename F>
    data_< image_if_base<I,F> >::data_(I& ima, const pset_if<mln_pset(I), F>& pset)
      : ima_(ima),
	pset_(pset)
    {
    }
    
    // image_if_base<I,F>
    
    template <typename I, typename F>
    image_if_base<I,F>::image_if_base()
    {
    }
    
    template <typename I, typename F>
    image_if_base<I,F>::image_if_base(I& ima, const F& f)
    {
      init_(ima, f);
    }
    
    template <typename I, typename F>
    void
    image_if_base<I,F>::init_(I& ima, const F& f)
    {
      mln_precondition(! this->has_data());
      this->data_ = new internal::data_< image_if_base<I,F> >(ima, f);
    }
    
    template <typename I, typename F>
    void
    image_if_base<I,F>::init_(I& ima, const pset_if<mln_pset(I), F>& pset)
    {
      mln_precondition(! this->has_data());
      this->data_ = new internal::data_< image_if_base<I,F> >(ima, pset);
    }
    
    template <typename I, typename F>
    bool
    image_if_base<I,F>::owns_(const mln_psite(I)& p) const
    {
      return this->data_->pset_.has(p);
    }
    
    template <typename I, typename F>
    const pset_if<mln_pset(I), F>&
    image_if_base<I,F>::domain() const
    {
      return this->data_->pset_;
    }
    
# endif // ! MLN_INCLUDE_ONLY
  
  } // end of namespace mln::internal
  
} // end of namespace mln
  
  
#endif // ! MLN_CORE_IMAGE_IF_BASE_HH
  
