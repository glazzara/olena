// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_PW_IMAGE_HH
# define MLN_PW_IMAGE_HH

/// \file
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

    /*!
      \internal
      \brief Data structure for mln::pw::internal::image
    */
    template <typename F, typename S>
    struct data< mln::pw::image<F,S> >
    {
      data(const F& f, const S& ps);
      F f_;
      S domain_;
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
    ///
    /// Parameter \p F is a function restricting the domain.
    /// Parameter \p S is the domain type.
    ///
    /// \ingroup modimageconcrete
    //
    template <typename F, typename S>
    class image :
      public pw::internal::image_base< F, S, image<F,S> >
    {
      typedef pw::internal::image_base< F, S, image<F,S> > super_;

    public:
      /// Skeleton.
      typedef image< tag::function_<F>, tag::domain_<S> > skeleton;

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
      domain_(ps)
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
