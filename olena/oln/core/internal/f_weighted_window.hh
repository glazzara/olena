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

#ifndef OLN_CORE_INTERNAL_F_WEIGHTED_WINDOW_HH
# define OLN_CORE_INTERNAL_F_WEIGHTED_WINDOW_HH

# include <oln/core/concept/dpoint.hh>
# include <oln/core/concept/weighted_window.hh>


# define oln_f_weighted_window(W, Dp) \
typename oln::internal::f_weighted_window_< W, Dp >::ret


namespace oln
{

  /// \{
  /// Forward declarations.

  // Dpoint types.

  struct dpoint1d;
  struct dpoint2d;
  struct dpoint3d;
  template <typename G, typename C> class dpoint;

  // Weighted window types.

  template <typename W> struct weighted_window2d;
  template <typename W, typename Dp> class weighted_window;

  /// \}



  namespace internal
  {

    template <typename W, typename Dp>
    struct weighted_window__;


    /// \{
    /// Definitions.

    template <typename W, typename G, typename C>
    struct weighted_window__< W, oln::dpoint<G, C> >
    {
      typedef weighted_window< W, oln::dpoint<G, C> > ret;
    };

    template <typename W>
    struct weighted_window__< W, dpoint2d >
    {
      typedef weighted_window2d<W> ret;
    };

    // FIXME: 1D, 3D, 2D hex/tri...

    /// \}


    template <typename W, typename Dp>
    struct f_weighted_window_ : private mlc::assert_< mlc_is_a(Dp, Dpoint) >,
				public weighted_window__<W, Dp>
    {
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_WEIGHTED_WINDOW_HH
