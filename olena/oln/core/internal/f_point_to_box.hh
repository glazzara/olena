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

#ifndef OLN_CORE_INTERNAL_F_POINT_TO_BOX_HH
# define OLN_CORE_INTERNAL_F_POINT_TO_BOX_HH

# include <oln/core/concept/point.hh>


# define oln_f_point_to_box(P) \
typename oln::internal::f_point_to_box_< P >::ret


namespace oln
{

  /// \{
  /// Forward declarations.

  // Point types.

  struct point1d;
  struct point2d;
  struct point3d;
  template <typename G, typename C> class point;

  // Box types.

  struct box1d;
  struct box2d;
  struct box3d;
  template <typename G, typename C> class box;

  /// \}



  namespace internal
  {

    template <typename P>
    struct point_to_box__;


    /// \{
    /// Definitions.

    template <typename G, typename C>
    struct point_to_box__< point<G, C> >
    {
      typedef box<G, C> ret;
    };

    template <>
    struct point_to_box__< point1d >
    {
      typedef box1d ret;
    };

    template <>
    struct point_to_box__< point2d >
    {
      typedef box2d ret;
    };

    // FIXME: 2D hex/tri...

    template <>
    struct point_to_box__< point3d >
    {
      typedef box3d ret;
    };

    /// \}


    template <typename P>
    struct f_point_to_box_ : private mlc::assert_< mlc_is_a(P, Point) >,
			     public point_to_box__< P >
    {
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_POINT_TO_BOX_HH
