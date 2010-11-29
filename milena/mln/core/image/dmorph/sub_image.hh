// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_HH
# define MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_HH

/// \file
///
/// Definition of morpher that makes an image become restricted
/// given by a point set.
///
/// \todo Add a special case for "ima | box"; think about some other
/// special cases...

# include <mln/core/internal/image_domain_morpher.hh>



namespace mln
{


  // Forward declaration.
  template <typename I, typename S> class sub_image;


  namespace internal
  {

    /// Data structure for \c mln::sub_image<I,S>.
    template <typename I, typename S>
    struct data< sub_image<I,S> >
    {
      data(const I& ima, const S& pset);

      I ima_;
      S domain_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename S>
    struct image_< sub_image<I,S> > : default_image_morpher< I,
                                                             mln_value(I),
                                                             sub_image<I,S> >
    {
//     private:
//       typedef mln_trait_image_data(I) I_data_;
//       typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
//     public:

      typedef trait::image::category::domain_morpher category;

      typedef trait::image::ext_domain::none      ext_domain;  // No extension of domain.
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_alignment::not_aligned   value_alignment;
      typedef trait::image::value_storage::disrupted value_storage;
      // HOT FIXME: except if S is a Box
    };

  } // end of namespace mln::trait



  /// Image having its domain restricted by a site set.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename S>
  class sub_image : public internal::image_domain_morpher< I,
							    S,
							    sub_image<I,S> >
  {
  public:
    /// Skeleton.
    typedef sub_image< tag::image_<I>, tag::domain_<S> > skeleton;

    /// Constructor without argument.
    sub_image();

    /// Constructor.
    sub_image(const I& ima, const S& pset);

    /// Initialization.
    void init_(const I& ima, const S& pset);

    /// Give the definition domain.
    const S& domain() const;

    /// Const promotion via conversion.
    operator sub_image<const I, S>() const;
  };





  template <typename I, typename S>
  sub_image<const I, S>
  operator|(const Image<I>& ima, const Site_Set<S>& pset);


  template <typename I, typename S>
  sub_image<I, S>
  operator|(Image<I>& ima, const Site_Set<S>& pset);



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


  // internal::data< sub_image<I,S> >

  namespace internal
  {

    template <typename I, typename S>
    inline
    data< sub_image<I,S> >::data(const I& ima, const S& pset)
      : ima_(ima),
	domain_(pset)
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
  sub_image<I,S>::sub_image(const I& ima, const S& pset)
  {
    init_(ima, pset);
  }

  template <typename I, typename S>
  inline
  void
  sub_image<I,S>::init_(const I& ima, const S& pset)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< sub_image<I,S> >(ima, pset);
  }

  template <typename I, typename S>
  inline
  const S&
  sub_image<I,S>::domain() const
  {
    return this->data_->domain_;
  }

  template <typename I, typename S>
  inline
  sub_image<I,S>::operator sub_image<const I, S>() const
  {
    sub_image<const I, S> tmp(this->data_->ima_,
			      this->data_->domain_);
    return tmp;
  }


  // Operators.

  template <typename I, typename S>
  inline
  sub_image<const I, S>
  operator|(const Image<I>& ima, const Site_Set<S>& pset)
  {
    sub_image<const I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

  template <typename I, typename S>
  inline
  sub_image<I, S>
  operator|(Image<I>& ima, const Site_Set<S>& pset)
  {
    sub_image<I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_HH
