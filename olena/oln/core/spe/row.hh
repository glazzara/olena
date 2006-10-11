// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_SPE_ROW_HH
# define OLN_CORE_SPE_ROW_HH

# include <mlc/value.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/abstract/point_set.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iterator_on_points.hh>


namespace oln
{

  template <typename P>
  oln_coord(P) row(const abstract::point<P>& p);

  template <typename P>
  oln_coord(P)& row(abstract::point<P>& p);


  template <typename D>
  oln_coord(D) row(const abstract::dpoint<D>& dp);

  template <typename D>
  oln_coord(D)& row(abstract::dpoint<D>& dp);


  template <typename It>
  oln_coord(It) row(const abstract::iterator_on_points<It>& it);


  template <typename Ps>
  oln_coord(Ps) min_row(const abstract::point_set_having_bbox<Ps>& ps);

  template <typename Ps>
  oln_coord(Ps) max_row(const abstract::point_set_having_bbox<Ps>& ps);


  template <typename I>
  oln_coord(I) min_row(const abstract::image_having_bbox<I>& ima);

  template <typename I>
  oln_coord(I) max_row(const abstract::image_having_bbox<I>& ima);



# ifndef OLN_INCLUDE_ONLY

  // From point.

  template <typename P>
  oln_coord(P) row(const abstract::point<P>& p)
  {
    typedef oln_type_of(P, row_comp) comp_t;
    return p.vec()[mlc_value(comp_t)];
  }

  template <typename P>
  oln_coord(P)& row(abstract::point<P>& p)
  {
    typedef oln_type_of(P, row_comp) comp_t;
    return p.vec()[mlc_value(comp_t)];
  }

  // From dpoint.

  template <typename D>
  oln_coord(D) row(const abstract::dpoint<D>& p)
  {
    typedef oln_type_of(D, row_comp) comp_t;
    return p.vec()[mlc_value(comp_t)];
  }

  template <typename D>
  oln_coord(D)& row(abstract::dpoint<D>& p)
  {
    typedef oln_type_of(D, row_comp) comp_t;
    return p.vec()[mlc_value(comp_t)];
  }

  // From iterator.

  template <typename P>
  oln_coord(P) row(const abstract::iterator_on_points<P>& it)
  {
    typedef oln_deduce_type_of(P, point, row_comp) comp_t;
    return it.to_point().vec()[mlc_value(comp_t)];
  }

  // From point set.

  template <typename Ps>
  oln_coord(Ps) min_row(const abstract::point_set_having_bbox<Ps>& ps)
  {
    typedef oln_deduce_type_of(Ps, point, row_comp) comp_t;
    return ps.pmin().vec()[mlc_value(comp_t)];
  }

  template <typename Ps>
  oln_coord(Ps) max_row(const abstract::point_set_having_bbox<Ps>& ps)
  {
    typedef oln_deduce_type_of(Ps, point, row_comp) comp_t;
    return ps.pmax().vec()[mlc_value(comp_t)];
  }

  // From image.

  template <typename I>
  oln_coord(I) min_row(const abstract::image_having_bbox<I>& ps)
  {
    typedef oln_deduce_type_of(I, point, row_comp) comp_t;
    return ps.pmin().vec()[mlc_value(comp_t)];
  }

  template <typename I>
  oln_coord(I) max_row(const abstract::image_having_bbox<I>& ps)
  {
    typedef oln_deduce_type_of(I, point, row_comp) comp_t;
    return ps.pmax().vec()[mlc_value(comp_t)];
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_SPE_ROW_HH
