// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef APPS_GRAPH_MORPHO_IMAGE_IF_LARGE_HH
# define APPS_GRAPH_MORPHO_IMAGE_IF_LARGE_HH

/// \file
///
/// \brief Definition of a image which domain is restricted by a
/// function 'site -> Boolean', but where routine has() returns true
/// for sites of the underlying (morphed) image even if they are not
/// validating the predicate.

// FIXME: This file is a workaround for the lack of ``tolerant''
// image_if, but there must be a better way.

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/site_set/p_if.hh>
# include <mln/pw/all.hh>


namespace mln
{

  // Forward declaration.
  template <typename I, typename F> struct image_if_large;


  namespace internal
  {

    /// Data structure for \c mln::image_if_large<I,F>.
    template <typename I, typename F>
    struct data< image_if_large<I,F> >
    {
      data(I& ima, const F& f);

      I ima_;
      p_if<mln_domain(I), F> domain_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename F>
    struct image_< image_if_large<I,F> >
      : default_image_morpher< I,
			       mln_value(I),
			       image_if_large<I,F> >
    {
      typedef trait::image::category::domain_morpher category;

      // No extension of domain.
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;

      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_alignment::not_aligned   value_alignment;
      typedef trait::image::value_storage::disrupted       value_storage;
    };

  } // end of namespace mln::trait



  /// Image which domain is restricted by a function 'site ->
  /// Boolean', but where routine has() returns true for sites of the
  /// underlying (morphed) image even if they are not validating the
  /// predicate.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename F>
  struct image_if_large
    : public mln::internal::image_domain_morpher< I,
						  p_if<mln_domain(I), F>,
						  image_if_large<I, F> >
  {
    typedef image_if_large<I, F> self;
    typedef mln_psite(self) psite;

    /// Skeleton.
    typedef image_if_large< tag::image_<I>, tag::function_<F> > skeleton;

    /// Constructor without argument.
    image_if_large();

    /// Constructor from an image \p ima and a predicate \p f.
    image_if_large(I& ima, const F& f);

    void init_(I& ima, const F& f);

    /// \brief Redefined psite membership method, returning true for
    /// sites of the underlying (morphed) image even if they are not
    /// validating the predicate.
    ///
    /// This is the only difference with mln::image_if.
    bool has(const psite& p) const;

    /// Give the definition domain.
    const p_if<mln_domain(I), F>& domain() const;

    /// Const promotion via conversion.
    operator image_if_large<const I, F>() const;
  };


  // Image || Function_v2b.

  /// ima || f creates an image_if_large with the image ima and the function
  /// f.
  //
  template <typename I, typename F>
  image_if_large<I,F>
  operator || (Image<I>& ima, const Function_v2b<F>& f);

  /// ima || f creates an image_if_large with the image ima and the function
  /// f.
  //
  template <typename I, typename F>
  image_if_large<const I,F>
  operator || (const Image<I>& ima, const Function_v2b<F>& f);



  template <typename I, typename A>
  image_if_large< const I, fun::C<bool(*)(A)> >
  operator || (const Image<I>& ima, bool (*f)(A) );

  template <typename I, typename A>
  image_if_large< I, fun::C<bool(*)(A)> >
  operator || (Image<I>& ima, bool (*f)(A) );

//   // Construction helpers.

//   template <typename I, typename A>
//   image_if_large< const I, fun::C<bool(*)(A)> >
//   make_image_if_large(const Image<I>& ima, bool (*f)(A) );

//   template <typename I, typename A>
//   image_if_large< I, fun::C<bool(*)(A)> >
//   make_image_if_large(Image<I>& ima, bool (*f)(A) );



# ifndef MLN_INCLUDE_ONLY

  // init_.

  template <typename I, typename F>
  void init_(tag::function_t, F& f, const image_if_large<I,F>& model)
  {
    f = model.domain().predicate();
  }

  template <typename I, typename F, typename J>
  void init_(tag::image_t, image_if_large<I,F>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, exact(model));
    F f;
    init_(tag::function, f, exact(model));
    target.init_(ima, f);
  }

  // internal::data< image_if_large<I,F> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< image_if_large<I,F> >::data(I& ima, const F& f)
      : ima_(ima),
	domain_(ima.domain() | f)
    {
    }

  }


  // image_if_large<I,F>

  template <typename I, typename F>
  inline
  image_if_large<I,F>::image_if_large()
  {
  }

  template <typename I, typename F>
  inline
  image_if_large<I,F>::image_if_large(I& ima, const F& f)
  {
    init_(ima, f);
  }

  template <typename I, typename F>
  inline
  void
  image_if_large<I,F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< image_if_large<I,F> >(ima, f);
  }

  template <typename I, typename F>
  inline
  bool
  image_if_large<I,F>::has(const typename image_if_large<I,F>::psite& p) const
  {
    mln_precondition(exact(this)->is_valid());
    // This method is more tolerant than mln::image_if's one (which
    // returns this->domain().has(p)).
    return this->data_->ima_.has(p);
  }


  template <typename I, typename F>
  inline
  const p_if<mln_domain(I), F>&
  image_if_large<I,F>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->domain_;
  }

  template <typename I, typename F>
  inline
  image_if_large<I,F>::operator image_if_large<const I,F>() const
  {
    mln_precondition(this->is_valid());
    image_if_large<const I,F> tmp(this->data_->ima_,
				  this->data_->domain_.predicate());
    return tmp;
  }


  // Operators.

  template <typename I, typename F>
  inline
  image_if_large<I,F>
  operator || (Image<I>& ima, const Function_v2b<F>& f)
  {
    image_if_large<I,F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  inline
  image_if_large<const I, F>
  operator || (const Image<I>& ima, const Function_v2b<F>& f)
  {
    image_if_large<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }


  template <typename I, typename A>
  image_if_large< const I, fun::C<bool(*)(A)> >
  operator || (const Image<I>& ima, bool (*f)(A) )
  {
    return exact(ima) || convert::to_fun(f);
  }

  template <typename I, typename A>
  image_if_large< I, fun::C<bool(*)(A)> >
  operator || (Image<I>& ima, bool (*f)(A) )
  {
    return exact(ima) || convert::to_fun(f);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! APPS_GRAPH_MORPHO_IMAGE_IF_LARGE_HH
