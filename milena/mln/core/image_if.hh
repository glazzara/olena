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

#ifndef MLN_CORE_IMAGE_IF_HH
# define MLN_CORE_IMAGE_IF_HH

/*! \file mln/core/image_if.hh
 *
 * \brief Definition of a base class for image adaptors.
 */

# include <mln/core/internal/image_adaptor.hh>
# include <mln/core/pset_if.hh>


namespace mln
{

  /*! \brief A base class for image adaptors.
   *
   */
  template <typename I, typename F>
  struct image_if : public internal::image_adaptor_< I,
						     image_if<I,F>,
						     pset_if<mln_pset(I),F>  >
  {
    /// Point_Set associated type.
    typedef pset_if<mln_pset(I), F> pset;

    /// Constructor from an \p adaptee image.
    image_if(I& adaptee, const F& f);

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_psite(I)& p) const;

    /// Give the definition domain.
    const pset& domain() const;

    template <typename T>
    struct change_value
    {
      typedef image_if<mln_ch_value(I,T), F> ret;
    };

  protected:

    pset pset_;
    F f_;

    typedef image_if<I,F> self_;
    typedef internal::image_adaptor_< I, self_, pset > super_;
  };



  template <typename I, typename F>
  image_if<I, F>
  operator | (Image<I>& ima, const Function_p2b<F>& f)
  {
    image_if<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }


# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename F>
  image_if<I,F>::image_if(I& adaptee, const F& f)
    : super_(adaptee),
      pset_(adaptee.domain() | f),
      f_(f)
  {
  }

  template <typename I, typename F>
  bool
  image_if<I,F>::owns_(const mln_psite(I)& p) const
  {
    return pset_.has(p);
  }

  template <typename I, typename F>
  const pset_if<mln_pset(I), F>&
  image_if<I,F>::domain() const
  {
    return pset_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IF_HH
