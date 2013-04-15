// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_COMPLEX_WINDOW_P_BASE_HH
# define MLN_CORE_INTERNAL_COMPLEX_WINDOW_P_BASE_HH

/// \file
/// \brief Definition of a generic window centered on the face
/// of a complex, based on a pair of (forward and backward) complex
/// iterators.  The center (site) is part of the window.

# include <mln/core/internal/complex_window_base.hh>

# include <mln/topo/centered_iter_adapter.hh>



namespace mln
{

  namespace internal
  {
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_window_p_base;
  }


  namespace trait
  {

    template <unsigned D, typename G, typename F, typename B, typename E>
    struct window_< mln::internal::complex_window_p_base<D, G, F, B, E> >
      : window_< mln::internal::complex_window_base
		 < D, G,
		   mln::topo::centered_fwd_iter_adapter<D, F>,
		   mln::topo::centered_bkd_iter_adapter<D, B>,
		   E > >
    {
    };

  } // end of namespace mln::trait


  namespace internal
  {
    /*!
      \internal
      \brief Generic window centered on the face of a complex, based
      on an pair of (forward and backward) complex iterators.  The
      center (site) is part of the window.

      \tparam D The dimension of the complex.
      \tparam G The type of the geometry functor of the complex.
      \tparam F The underlying forward iterator type.
      \tparam B The underlying backward iterator type.
      \tparam E The exact type.
    */
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_window_p_base
      : public complex_window_base
               < D, G,
		 mln::topo::centered_fwd_iter_adapter<D, F>,
		 mln::topo::centered_bkd_iter_adapter<D, B>,
		 E >
    {
      typedef complex_window_base< D, G,
				   topo::centered_fwd_iter_adapter<D, F>,
				   topo::centered_bkd_iter_adapter<D, B>,
				   E > super;

    public:
      complex_window_p_base();
    };



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G, typename F, typename B, typename E>
    complex_window_p_base<D, G, F, B, E>::complex_window_p_base()
      : super(true)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_COMPLEX_WINDOW_P_BASE_HH
