// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_COMPLEX_WINDOWS_P_HH
# define MLN_CORE_IMAGE_COMPLEX_WINDOWS_P_HH

/// \file mln/core/image/complex_windows.hh
/// \brief Definitions of some windows of complex images.

# include <mln/core/internal/complex_window_p_base.hh>

# include <mln/topo/adj_lower_face_iter.hh>
# include <mln/topo/adj_higher_face_iter.hh>
# include <mln/topo/adj_lower_higher_face_iter.hh>


namespace mln
{

  /// \brief Window centered on an n-face of complex returning its
  /// adjacent (n-1)-faces as well as the center n-face.
  template <unsigned D, typename G>
  struct complex_lower_window_p
    : internal::complex_window_p_base< D, G,
				       topo::adj_lower_face_fwd_iter<D>,
				       topo::adj_lower_face_bkd_iter<D>,
				       complex_lower_window_p<D, G> >
  {
  };

  /// \brief Window centered on an n-face of complex returning its
  /// adjacent (n+1)-faces as well as the center n-face.
  template <unsigned D, typename G>
  struct complex_higher_window_p
    : internal::complex_window_p_base< D, G,
				       topo::adj_higher_face_fwd_iter<D>,
				       topo::adj_higher_face_bkd_iter<D>,
				       complex_higher_window_p<D, G> >
  {
  };

  /// \brief Window centered on an n-face of complex returning its
  /// adjacent (n-1)-faces and (n+1)-faces as well as the center
  /// n-face.
  template <unsigned D, typename G>
  struct complex_lower_higher_window_p
    : internal::complex_window_p_base< D, G,
				       topo::adj_lower_higher_face_fwd_iter<D>,
				       topo::adj_lower_higher_face_bkd_iter<D>,
				       complex_lower_higher_window_p<D, G> >
  {
  };


  namespace trait
  {

    template <unsigned D, typename G>
    struct window_< mln::complex_lower_window_p<D, G> >
      : window_< mln::internal::complex_window_p_base<
		      D, G,
		      mln::topo::adj_lower_face_fwd_iter<D>,
		      mln::topo::adj_lower_face_bkd_iter<D>,
		      mln::complex_lower_window_p<D, G> >
		 >
    {
    };

    template <unsigned D, typename G>
    struct window_< mln::complex_higher_window_p<D, G> >
      : window_< mln::internal::complex_window_p_base<
		      D, G,
		      mln::topo::adj_higher_face_fwd_iter<D>,
		      mln::topo::adj_higher_face_bkd_iter<D>,
		      mln::complex_higher_window_p<D, G> >
		 >
    {
    };

    template <unsigned D, typename G>
    struct window_< mln::complex_lower_higher_window_p<D, G> >
      : window_< mln::internal::complex_window_p_base<
		      D, G,
		      mln::topo::adj_lower_higher_face_fwd_iter<D>,
		      mln::topo::adj_lower_higher_face_bkd_iter<D>,
		      mln::complex_lower_higher_window_p<D, G> >
		 >
    {
    };

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_WINDOWS_P_HH
