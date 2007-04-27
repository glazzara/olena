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

#ifndef OLN_CORE_GEN_WINDOW_HH
# define OLN_CORE_GEN_WINDOW_HH

# include <oln/core/internal/window.hh>


namespace oln
{


  // Fwd decl.
  template <typename Dp> class gen_window;


  // Super type.
  template <typename Dp>
  struct super_trait_< gen_window<Dp> >
  {
    typedef gen_window<Dp> current__;
    typedef internal::window_<current__> ret;
  };


  // Virtual types.
  template <typename Dp>
  struct vtypes< gen_window<Dp> >
  {
    typedef oln_point(Dp) point;
  };


  /// Generic classical window class.

  template <typename Dp>
  class gen_window : public internal::window_< gen_window<Dp> >
  {
  public:
    
    gen_window();

    template <unsigned n>
    void impl_fill_with(const bool (&values)[n]);

  }; // end of class oln::gen_window<Dp>



# ifndef OLN_INCLUDE_ONLY

  template <typename Dp>
  gen_window<Dp>::gen_window()
  {
  }

# endif // ! OLN_INCLUDE_ONLY
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_WINDOW_HH
