// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file mln/pw/image.hh
///
/// Point-wised restricted image.

# include <mln/pw/internal/image_base.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Forward declaration.
  namespace pw { template <typename F, typename S> class image; }


  // Properties

  namespace trait
  {

    template <typename F, typename S>
    struct image_< pw::image<F,S> >
      : pw_image_<F,S, pw::image<F,S> >
    {
    };

  } // end of namespace mln::traits.


  namespace internal
  {

    /// Data structure for mln::pw::internal::image
    template <typename F, typename S>
    struct data< mln::pw::image<F,S> >
    {
      data(const F& f, const S& ps);
      F f_;
      S pset_;
    };

  } // end of namespace mln::internal


  /// Construct an image from a function and a site set.
  /// image = function | site_set.
  template <typename F, typename S>
  pw::image<F,S>
  operator | (const Function_v2v<F>& f, const Site_Set<S>& ps);


  namespace pw
  {

    /// A generic point-wise image implementation.
    /// Parameter \p F is a function restricting the domain.
    /// Parameter \p S is the domain type.
    template <typename F, typename S>
    class image :
      public pw::internal::image_base< F, S, image<F,S> >
    {
      typedef pw::internal::image_base< F, S, image<F,S> > super_;

    public:
      /// Skeleton.
      typedef image< tag::function_<F>, tag::pset_<S> > skeleton;

      /// Constructor without argument.
      image();

      /// Constructor.
      image(const Function_v2v<F>& f, const Site_Set<S>& ps);

    };

  } // end of namespace mln::pw


  /// Init_

  template <typename F, typename S, typename E, typename J>
  void init_(tag::image_t, pw::image<F,S>& target, const Image<J>& model);


# ifndef MLN_INCLUDE_ONLY

  /// Init_

  template <typename F, typename S, typename E, typename J>
  void init_(tag::image_t, pw::image<F,S>& target, const Image<J>& model)
  {
    F f;
    init_(tag::function, f, exact(model));
    S s;
    init_(tag::domain, s, exact(model));
    target.init_(f, s);
  }


  // Operator.

  template <typename F, typename S>
  inline
  pw::image<F,S>
  operator | (const Function_v2v<F>& f, const Site_Set<S>& ps)
  {
    pw::image<F,S> tmp(f, ps);
    return tmp;
  }


  // data< pw::image >

  namespace internal
  {

    template <typename F, typename S>
    inline
    data< mln::pw::image<F,S> >::data(const F& f, const S& ps)
      : f_(f),
      pset_(ps)
    {
    }

  } // end of namespace mln::internal



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
    image<F,S>::image(const Function_v2v<F>& f, const Site_Set<S>& ps)
    {
      this->data_ = new mln::internal::data< image<F,S> >(exact(f), exact(ps));
    }


  } // end of namespace mln::pw

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_PW_IMAGE_HH
