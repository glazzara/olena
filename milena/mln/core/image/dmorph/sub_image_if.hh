// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_IF_HH
# define MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_IF_HH


/// \file
///
/// Image having its domain restricted by a site set and a function.
///
/// \todo Activate preconditions.


# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/site_set/p_if.hh>
# include <mln/fun/p2b/has.hh>

# include <mln/debug/println.hh>


namespace mln
{


  // Forward declaration.
  template <typename I, typename S> struct sub_image_if;


  namespace internal
  {

    /// Data structure for \c mln::sub_image_if<I,S>.
    template <typename I, typename S>
    struct data< sub_image_if<I,S> >
    {
      data(I& ima, const S& s);

      I ima_;
      p_if< S, fun::p2b::has<I> > s_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename S>
    struct image_< sub_image_if<I,S> > : default_image_morpher< I,
								mln_value(I),
								sub_image_if<I,S> >
    {
//     private:
//       typedef mln_trait_image_data(I) I_data_;
//       typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
//     public:

      typedef trait::image::category::domain_morpher category;

      typedef trait::image::ext_domain::none      ext_domain;  // No extension of domain.
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::value_storage::disrupted value_storage;
      // HOT FIXME: except if S is a Box
    };

  } // end of namespace mln::trait



  /// Image having its domain restricted by a site set and a function.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename S>
  struct sub_image_if : public internal::image_domain_morpher< I,
							       p_if< S, fun::p2b::has<I> >,
							       sub_image_if<I,S> >
  {
    /// Skeleton.
    typedef sub_image_if< tag::image_<I>, tag::domain_<S> > skeleton;

    /// Constructor without argument.
    sub_image_if();

    /// Constructor.
    sub_image_if(I& ima, const S& s);

    /// Initialization.
    void init_(I& ima, const S& s);

    /// Give the definition domain.
    const p_if< S, fun::p2b::has<I> >& domain() const;

//     /// Const promotion via conversion.
//     operator sub_image_if<const I, S>() const;
  };





  template <typename I, typename S>
  sub_image_if<const I, S>
  operator / (const Image<I>& ima, const Site_Set<S>& s);


  template <typename I, typename S>
  sub_image_if<I, S>
  operator / (Image<I>& ima, const Site_Set<S>& s);


  template <typename I, typename S, typename J>
  void init_(tag::image_t, sub_image_if<I,S>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename I, typename S, typename J>
  inline
  void init_(tag::image_t, sub_image_if<I,S>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    S s;
    init_(tag::domain, s, model);
    target.init_(ima, s);
  }


  // internal::data< sub_image_if<I,S> >

  namespace internal
  {

    template <typename I, typename S>
    inline
    data< sub_image_if<I,S> >::data(I& ima, const S& s)
      : ima_(ima)
    {
      typedef fun::p2b::has<I> F;
      F f(ima);
      s_ = p_if<S,F>(s, f);
    }

  } // end of namespace mln::internal


  // sub_image_if<I,S>

  template <typename I, typename S>
  inline
  sub_image_if<I,S>::sub_image_if()
  {
  }

  template <typename I, typename S>
  inline
  sub_image_if<I,S>::sub_image_if(I& ima, const S& s)
  {
    init_(ima, s);
  }

  template <typename I, typename S>
  inline
  void
  sub_image_if<I,S>::init_(I& ima, const S& s)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< sub_image_if<I,S> >(ima, s);
  }

  template <typename I, typename S>
  inline
  const p_if< S, fun::p2b::has<I> >&
  sub_image_if<I,S>::domain() const
  {
    return this->data_->s_;
  }

//   template <typename I, typename S>
//   inline
//   sub_image_if<I,S>::operator sub_image_if<const I, S>() const
//   {
//     sub_image_if<const I, S> tmp(this->data_->ima_,
// 			      this->data_->s_);
//     return tmp;
//   }


  // Operators.

  template <typename I, typename S>
  inline
  sub_image_if<const I, S>
  operator / (const Image<I>& ima, const Site_Set<S>& s)
  {
    // FIXME: mln_precondition(set::inter(ima.domain(), s) != 0);
    sub_image_if<const I, S> tmp(exact(ima), exact(s));
    return tmp;
  }

  template <typename I, typename S>
  inline
  sub_image_if<I, S>
  operator / (Image<I>& ima, const Site_Set<S>& s)
  {
    // FIXME: mln_precondition(set::inter(ima.domain(), s) != 0);
    sub_image_if<I, S> tmp(exact(ima), exact(s));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_SUB_IMAGE_IF_HH
