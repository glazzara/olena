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

#ifndef OLN_CORE_GEN_RESTRICTED_TO_HH
# define OLN_CORE_GEN_RESTRICTED_TO_HH


namespace oln
{
  struct restricted_to;
}


# include <oln/core/internal/op_image_restricted_to_pset.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{

  // Image | Point_Set

  template <typename I, typename S>
  op_<const I, restricted_to, const S>
  operator | (const Image<I>& ima, const Point_Set<S>& pset);

  template <typename I, typename S>
  op_<I, restricted_to, const S>
  operator | (Mutable_Image<I>& ima, const Point_Set<S>& pset);

  template <typename I, typename S>
  inplace_< op_<I, restricted_to, const S> >
  operator | (inplace_<I> ima, const Point_Set<S>& pset);



# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename S>
  op_<const I, restricted_to, const S>
  operator | (const Image<I>& ima, const Point_Set<S>& pset)
  {
    mlc::assert_equal_< oln_point(S), oln_point(I) >::check();
    precondition(pset < ima.points());
    op_<const I, restricted_to, const S> tmp(exact(ima), exact(pset));
    return tmp;
  }

  template <typename I, typename S>
  op_<I, restricted_to, const S>
  operator | (Mutable_Image<I>& ima, const Point_Set<S>& pset)
  {
    mlc::assert_equal_< oln_point(S), oln_point(I) >::check();
    precondition(pset < ima.points());
    op_<I, restricted_to, const S> tmp(exact(ima), exact(pset));
    return tmp;
  }

  template <typename I, typename S>
  inplace_< op_<I, restricted_to, const S> >
  operator | (inplace_<I> ima, const Point_Set<S>& pset)
  {
    return (ima.unwrap() | pset).inplace();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_RESTRICTED_TO_HH
