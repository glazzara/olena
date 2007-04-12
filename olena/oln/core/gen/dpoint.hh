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

#ifndef OLN_CORE_GEN_DPOINT_HH
# define OLN_CORE_GEN_DPOINT_HH

# include <oln/core/internal/dpoint_base.hh>
# include <oln/core/internal/point_impl.hh>


namespace oln
{

# define current dpoint<G, C>

  // Fwd decls.
  template <typename G, typename C> struct dpoint;
  template <typename G, typename C> struct  point;


  // Super type.
  template<typename G, typename C>
  struct super_trait_< current >
  {
    typedef internal::dpoint_base_< current > ret;
  };


  // Virtual types.
  template <typename G, typename C>
  struct vtypes< current >
  {
    typedef G grid;
    typedef C coord;
    typedef point<G,C> point;
  };


  template <typename G, typename C>
  class dpoint : public internal::dpoint_base_< current >,
		 public internal::point_impl_< mlc_value(oln_dim(G)), current >
  {
  public:
    dpoint();
    // nothing else here
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename G, typename C>
  current::dpoint()
  {
    mlc::assert_< mlc_is_a(G, Grid) >::check();
  }

# endif // ! OLN_INCLUDE_ONLY

# undef current

} // end of namespace oln


# include <oln/core/gen/point.hh>


#endif // ! OLN_CORE_GEN_DPOINT_HH
