// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TRAIT_WINDOWS_HH
# define MLN_TRAIT_WINDOWS_HH

/*! \file
 *
 * \brief Some base trait types for windows.
 */

# include <mln/trait/undef.hh>
# include <mln/trait/window/props.hh>


/// Shortcut to the window property about the 'size' method presence.
# define mln_trait_window_size(W)   typename mln::trait::window_< W >::size

/// Shortcut to the window property about the 'support' it is designed for.
# define mln_trait_window_support(W)     typename mln::trait::window_< W >::support

/// Shortcut to the window property about its definition.
# define mln_trait_window_definition(W) typename mln::trait::window_< W >::definition


# define mln_internal_add_classical_window_trait(W)	\
							\
    namespace win { struct W; }				\
							\
    namespace trait					\
    {							\
							\
      template <>					\
      struct window_< win::W > : classical_window_	\
      {							\
      };						\
							\
    }							\
							\
    struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


namespace mln
{

  namespace trait
  {

    /// Pack of 'undefined' type values for properties of windows.
    template <typename W>
    struct undefined_window_
    {
      typedef undef size;       // Fixed or unknown.
      typedef undef support;    // Regular or irregular.
      typedef undef definition; // Unique, n_ary, or varying.
    };


    /*! \brief The trait pack structure for properties of windows.
     *
     * This structure is specialized for every concrete class of site
     * set so that properties are properly defined.
     *
     * \see mln::doc::Window for the documentation of the "window"
     * concept.
     */
    template <typename W>
    struct window_ : undefined_window_<W>
    {
    };


    // \internal Trait for classical windows. 
    struct classical_window_
    {
      typedef mln::trait::window::size::fixed        size;
      typedef mln::trait::window::support::regular   support;
      typedef mln::trait::window::definition::unique definition;
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/window/print.hh>


#endif // ! MLN_TRAIT_WINDOWS_HH
