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

#ifndef MLN_CORE_SUBIMAGE_HH
# define MLN_CORE_SUBIMAGE_HH

/*! \file mln/core/subimage.hh
 *
 * \brief Definition of a base class for image adaptors.
 */

# include <mln/core/internal/image_adaptor.hh>
# include <mln/core/psubset.hh>


namespace mln
{

  /*! \brief A base class for image adaptors.
   *
   */
  template <typename I, typename F>
  struct subimage : public internal::image_adaptor_< I,
						     subimage<I,F>,
						     psubset<mln_pset(I),F>  >
  {
    /// Point_Set associated type.
    typedef psubset<mln_pset(I), F> pset;

    /// Constructor from an \p adaptee image.
    subimage(I& adaptee, const F& f);

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_psite(I)& p) const;

    /// Give the definition domain.
    const pset& domain() const;

    template <typename T>
    struct change_value
    {
      typedef subimage<mln_ch_value(I,T), F> ret;
    };

  protected:

    pset pset_;
    F f_;

    typedef subimage<I,F> self_;
    typedef internal::image_adaptor_< I, self_, pset > super_;
  };



  template <typename I, typename F>
  subimage<I, F>
  operator | (Image<I>& ima, const Function_p2b<F>& f)
  {
    subimage<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }


# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename F>
  subimage<I,F>::subimage(I& adaptee, const F& f)
    : super_(adaptee),
      pset_(adaptee.domain() | f),
      f_(f)
  {
  }

  template <typename I, typename F>
  bool
  subimage<I,F>::owns_(const mln_psite(I)& p) const
  {
    return pset_.has(p);
  }

  template <typename I, typename F>
  const psubset<mln_pset(I), F>&
  subimage<I,F>::domain() const
  {
    return pset_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SUBIMAGE_HH
