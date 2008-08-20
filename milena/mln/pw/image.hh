// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_PW_IMAGE_HH
# define MLN_PW_IMAGE_HH

/*! \file mln/pw/image.hh
 *
 * \brief FIXME.
 *
 * \todo Relax Function_p2v into Function_v2v.
 */

# include <mln/core/internal/image_primary.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  // Fwd decl.
  namespace pw { template <typename F, typename S> struct image; }



  /*! \brief FIXME
   *
   */
  template <typename F, typename S>
  pw::image<F,S>
  operator | (const Function_p2v<F>& f, const Site_Set<S>& ps);



  namespace internal
  {

    /// \internal Data structure for mln::pw::image
    template <typename F, typename S>
    struct data< mln::pw::image<F,S> >
    {
      data(const F& f, const S& ps);
      F f_;
      S pset_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    // FIXME: the result type should *not* be qualified

    template <typename F, typename S>
    struct image_< pw::image<F,S> > : default_image_< mlc_unqualif(mln_result(F)),
						      pw::image<F,S> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::value_access::computed         value_access;
      typedef trait::image::value_storage::disrupted       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_io::read_only            value_io;

      // site / domain
      typedef /* FIXME: depends on S */  undef  localization;
      typedef /* FIXME: depends on S */  undef  dimension;

      // extended domain
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait


  namespace pw
  {

    /*! \brief FIXME
     *
     */
    template <typename F, typename S>
    struct image : public internal::image_primary< S, image<F,S> >
    {
      /// Skeleton.
      typedef image< tag::function_<F>, tag::pset_<S> > skeleton;


      /// Value associated type.
      typedef mln_result(F) value;

      /// Return type of read-only access.
      typedef mln_result(F) rvalue;

      /// Return type of read-write access.
      typedef void lvalue; // FIXME

      /// Value set associated type.
      typedef mln::value::set<mln_result(F)> vset;


      /// Constructor without argument.
      image();

      /// Constructor.
      image(const Function_p2v<F>& f, const Site_Set<S>& ps);

      // No init_ method here since this image type is not "concrete".

      /// Give the definition domain.
      const S& domain() const;

      /// Read-only access of pixel value at point site \p p.
      mln_result(F) operator()(const mln_psite(S)& p) const;

      /// Read-write access is present but disabled.
      void operator()(const mln_psite(S)&);

      /// Give the set of values of the image.
      const vset& values() const;
    };

  } // end of namespace mln::pw



# ifndef MLN_INCLUDE_ONLY

  // Operator.

  template <typename F, typename S>
  inline
  pw::image<F,S>
  operator | (const Function_p2v<F>& f, const Site_Set<S>& ps)
  {
    pw::image<F,S> tmp(f, ps);
    return tmp;
  }

  // internal::data< pw::image<F,S> >

  namespace internal
  {
    /// \internal
    template <typename F, typename S>
    inline
    data< pw::image<F,S> >::data(const F& f, const S& ps)
      : f_(f),
	pset_(ps)
    {
    }

  }

  // pw::image<F,S>

  namespace pw
  {

    template <typename F, typename S>
    inline
    image<F,S>::image()
    {
    }

    template <typename F, typename S>
    inline
    image<F,S>::image(const Function_p2v<F>& f, const Site_Set<S>& ps)
    {
      this->data_ = new internal::data< pw::image<F,S> >(exact(f), exact(ps));
    }

    template <typename F, typename S>
    inline
    const S&
    image<F,S>::domain() const
    {
      return this->data_->pset_;
    }

    template <typename F, typename S>
    inline
    mln_result(F)
      image<F,S>::operator()(const mln_psite(S)& p) const
    {
      mln_precondition(this->data_->pset_.has(p));
      return this->data_->f_(p);
    }

    template <typename F, typename S>
    inline
    void
    image<F,S>::operator()(const mln_psite(S)&)
    {
      mln_invariant(0); // FIXME: Turn into a compile-time error...
    }

    template <typename F, typename S>
    inline
    const mln::value::set<mln_result(F)>&
    image<F,S>::values() const
    {
      return vset::the();
    }

  } // end of namespace mln::pw

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_PW_IMAGE_HH
