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

# include <oln/core/concept/point.hh>
# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/box.hh>


namespace oln
{


  // Fwd decl.
  template <typename P> class gen_box;


  // Super type.
  template <typename P>
  struct super_trait_< gen_box<P> >
  {
    typedef gen_box<P> current__;
    typedef internal::box_<current__> ret;
  };


  /// Virtual types.
  template <typename P>
  struct vtypes< gen_box<P> >
  {
    typedef P point;
    typedef gen_box<P> box;
  };


  /// Generic box class based on a point class.

  template <typename P>
  class gen_box : public internal::box_< gen_box<P> >,
		  private mlc::assert_< mlc_is_a(P, Point) >
  {
    typedef gen_box<P> current;
    typedef internal::box_<current> super;
  public:
    stc_using(point);
    stc_using(box);


  public:

    gen_box();
    gen_box(const P& pmin, const P& pmax);
    gen_box(const typename gen_box<P>::from_to_t& data);

    template <typename D>
    gen_box(const internal::initializer_<D>& data);

  }; // end of class oln::gen_box<P>



# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  gen_box<P>::gen_box()
  {
  }

  template <typename P>
  gen_box<P>::gen_box(const P& pmin, const P& pmax) :
    super(pmin, pmax)
  {
  }

  template <typename P>
  gen_box<P>::gen_box(const typename gen_box<P>::from_to_t& data) :
    super(data)
  {
  }

  template <typename P>
  template <typename D>
  gen_box<P>::gen_box(const internal::initializer_<D>& data) :
    super(data)
  {
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_BOX_HH
