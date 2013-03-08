// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_DMORPH_EXTENDED_HH
# define MLN_CORE_IMAGE_DMORPH_EXTENDED_HH

/// \file
///
/// Definition of morpher that makes an image become restricted
/// given by a point set.
///
/// \todo Add a special case for "ima | box"; think about some other
/// special cases...

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/site_set/box.hh>


namespace mln
{


  // Forward declaration.
  template <typename I> struct extended;


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::extended<I>.
    */
    template <typename I>
    struct data< extended<I> >
    {
      data(I& ima, const box<mln_site(I)>& b_);

      I ima_;
      box<mln_site(I)> b_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I>
    struct image_< extended<I> > : default_image_morpher< I,
							  mln_value(I),
							  extended<I> >
    {
      typedef trait::image::category::domain_morpher category;

      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::value_storage::disrupted value_storage;
    };

  } // end of namespace mln::trait



  /// Makes an image become restricted by a point set.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I>
  struct extended : public internal::image_domain_morpher< I,
							   box<mln_site(I)>,
							   extended<I> >,
		    private mlc_not_equal(mln_trait_image_ext_domain(I),
					  trait::image::ext_domain::none)::check_t
  {
    /// Value type.
    typedef mln_value(I) value;

    /// Skeleton.
    typedef tag::image_<I> skeleton; // This property is lost!

    /// Constructor without argument.
    extended();

    /// Constructor.
    extended(I& ima, const box<mln_site(I)>& b);

    /// Initialization.
    void init_(I& ima, const box<mln_site(I)>& b);

    /// Give the definition domain.
    const box<mln_site(I)>& domain() const;
  };



  template <typename I, typename J>
  void init_(tag::image_t, extended<I>& target, const J& model);


  template <typename I, typename B>
  extended<const I>
  extended_to(const Image<I>& ima, const Box<B>& b);

  template <typename I, typename B>
  extended<I>
  extended_to(Image<I>& ima, const Box<B>& b);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename I, typename J>
  inline
  void init_(tag::image_t, extended<I>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    box<mln_site(I)> b;
    init_(tag::bbox, b, model);
    target.init_(ima, b);
  }


  // internal::data< extended<I> >

  namespace internal
  {

    template <typename I>
    inline
    data< extended<I> >::data(I& ima, const box<mln_site(I)>& b)
      : ima_(ima),
	b_(b)
    {
    }

  } // end of namespace mln::internal


  // extended<I>

  template <typename I>
  inline
  extended<I>::extended()
  {
  }

  template <typename I>
  inline
  extended<I>::extended(I& ima, const box<mln_site(I)>& b)
  {
    init_(ima, b);
  }

  template <typename I>
  inline
  void
  extended<I>::init_(I& ima, const box<mln_site(I)>& b)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< extended<I> >(ima, b);
  }

  template <typename I>
  inline
  const box<mln_site(I)>&
  extended<I>::domain() const
  {
    return this->data_->b_;
  }


  // extended_to

  template <typename I, typename B>
  extended<const I>
  extended_to(const Image<I>& ima, const Box<B>& b)
  {
    mlc_not_equal(mln_trait_image_ext_domain(I),
		  trait::image::ext_domain::none)::check();
    mln_precondition(exact(ima).is_valid());
    extended<const I> tmp(exact(ima), exact(b));
    return tmp;
  }

  template <typename I, typename B>
  extended<I>
  extended_to(Image<I>& ima, const Box<B>& b)
  {
    mlc_not_equal(mln_trait_image_ext_domain(I),
		  trait::image::ext_domain::none)::check();
    mln_precondition(exact(ima).is_valid());
    extended<I> tmp(exact(ima), exact(b));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_EXTENDED_HH
