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

#ifndef OLN_CORE_GEN_INFTY_PSET_HH
# define OLN_CORE_GEN_INFTY_PSET_HH

# include <limits>
# include <ostream>

# include <oln/core/concept/grid.hh>
# include <oln/core/internal/f_grid_to_box.hh>
# include <oln/core/internal/point_set_base.hh>


namespace oln
{

  // Fwd decl.
  template <typename G> class infty_pset;


  // Super type.
  template <typename G>
  struct super_trait_< infty_pset<G> >
  {
    typedef infty_pset<G> current__;
    typedef internal::point_set_base_<current__> ret;
  };


  // Virtual types.
  template <typename G>
  struct vtypes< infty_pset<G> >
  {
    typedef oln_f_grid_to_box(G) box;
    typedef oln_point(box) point;

    // disabled!
    typedef void fwd_piter;
    typedef void bkd_piter;
  };


  /// Infinite point set defined over a grid.

  template <typename G>
  class infty_pset : public internal::point_set_base_< infty_pset<G> >
  {
    typedef infty_pset<G> current;
    typedef internal::point_set_base_<current> super;

  public:

    stc_using(point);
    stc_using(box);
      
    infty_pset();

    unsigned impl_npoints() const;
    bool impl_has(const point&) const;
    const box& impl_bbox() const;

  }; // end of class oln::infty_pset<G>.


  template <typename G>
  std::ostream& operator<<(std::ostream& ostr, const infty_pset<G>& ps);


# ifndef OLN_INCLUDE_ONLY

  template <typename G>
  infty_pset<G>::infty_pset()
  {
  }

  template <typename G>
  unsigned
  infty_pset<G>::impl_npoints() const
  {
    return std::numeric_limits<unsigned>::max();
  }

  template <typename G>
  bool
  infty_pset<G>::impl_has(const point&) const
  {
    return true;
  }

  template <typename G>
  const typename infty_pset<G>::box&
  infty_pset<G>::impl_bbox() const
  {
    return box::infty();
  }

  template <typename G>
  std::ostream& operator<<(std::ostream& ostr, const infty_pset<G>&)
  {
    return ostr << "infty pset";
  }

#endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_INFTY_PSET_HH
