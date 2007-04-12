// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_GEN_BOX_HH
# define OLN_CORE_GEN_BOX_HH

# include <oln/core/internal/box.hh>
# include <oln/core/gen/point.hh>


namespace oln
{

# define current box<G,C>


  // Fwd decl.
  template <typename G, typename C> class box;


  // Super type.
  template <typename G, typename C>
  struct super_trait_< current >
  {
    typedef internal::box_< current > ret;
  };


  // Virtual types.
  template <typename G, typename C>
  struct vtypes< current >
  {
    typedef point<G, C> point;
    typedef current box;
  };


  /// Generic box class based on a point class.

  template <typename G, typename C>
  class box : public internal::box_< current >
  {
    typedef internal::box_< current > super;
  public:
    stc_using(from_to_t);

    box();

    box(const oln::point<G,C>& pmin, const oln::point<G,C>& pmax);
    box(const from_to_t& data);

    template <typename D>
    box(const internal::initializer_<D>& data);

  }; // end of class oln::box<G,C>



# ifndef OLN_INCLUDE_ONLY

  template <typename G, typename C>
  current::box()
  {
    mlc::assert_< mlc_is_a(G, Grid) >::check();
  }

  template <typename G, typename C>
  current::box(const oln::point<G,C>& pmin,
	       const oln::point<G,C>& pmax)
    : super(pmin, pmax)
  {
  }

  template <typename G, typename C>
  current::box(const typename current::from_to_t& data) :
    super(data)
  {
  }

  template <typename G, typename C>
  template <typename D>
  current::box(const internal::initializer_<D>& data) :
    super(data)
  {
  }

# endif // ! OLN_INCLUDE_ONLY

# undef current

} // end of namespace oln


#endif // ! OLN_CORE_GEN_BOX_HH
