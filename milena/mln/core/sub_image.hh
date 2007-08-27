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

#ifndef MLN_CORE_SUB_IMAGE_HH
# define MLN_CORE_SUB_IMAGE_HH

# include <mln/core/internal/image_adaptor.hh>


namespace mln
{

  // FIXME: Doc!

  template <typename I, typename S>
  class sub_image : public internal::image_adaptor_< I,
						     sub_image<I,S>,
						     S >
  {
    typedef internal::image_adaptor_<I, sub_image<I,S>, S> super_;
  public:

    sub_image(I& ima, const S& pset);

    bool owns_(const mln_psite(I)& p) const;

    template <typename U>
    struct change_value
    {
      typedef internal::fixme ret;
    };

    const S& domain() const;

    /// Const promotion via convertion.
    operator sub_image<const I, S>() const;

  protected:
    const S& pset_;
  };



  template <typename I, typename S>
  sub_image<const I, S> operator|(const Image<I>& ima, const Point_Set<S>& pset);

  template <typename I, typename S>
  sub_image<I, S> operator|(Image<I>& ima, const Point_Set<S>& pset);



# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename S>
  sub_image<I,S>::sub_image(I& ima, const S& pset)
    : super_(ima),
      pset_(pset)
  {
  }

  template <typename I, typename S>
  bool
  sub_image<I,S>::owns_(const mln_psite(I)& p) const
  {
    return this->domain().has(p);
  }

  template <typename I, typename S>
  const S&
  sub_image<I,S>::domain() const
  {
    return pset_;
  }

  template <typename I, typename S>
  sub_image<I,S>::operator sub_image<const I, S>() const
  {
    sub_image<const I, S> tmp(this->adaptee_, this->pset_);
    return tmp;
  }

  // operator

  template <typename I, typename S>
  sub_image<const I, S>
  operator|(const Image<I>& ima, const Point_Set<S>& pset)
  {
    sub_image<const I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

  template <typename I, typename S>
  sub_image<I, S>
  operator|(Image<I>& ima, const Point_Set<S>& pset)
  {
    sub_image<I, S> tmp(exact(ima), exact(pset));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SUB_IMAGE_HH
