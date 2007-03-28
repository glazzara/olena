// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_CONCEPT_POINT_SET_HH
# define OLN_CORE_CONCEPT_POINT_SET_HH

# include <iostream>
# include <oln/core/concept/iterator_on_points.hh>


namespace oln
{

  /// Instant value.
  oln_decl_instant_value(box);


  /// Concept-class "Point_Set".

  template <typename Exact>
  struct Point_Set : public Any<Exact>
  {
    stc_typename(point);
    stc_typename(grid);
    stc_typename(piter);
    stc_typename(fwd_piter);
    stc_typename(bkd_piter);
    stc_typename(box);

    // FIXME: Add coord and dim?

    unsigned npoints() const;
    bool has(const point& p) const;
    const box& bbox() const;

  protected:
    Point_Set();

  private:
    void check__() const;

  }; // end of oln::Point_Set<Exact>


  /// Concept-class "Box".

  template <typename Exact>
  struct Box : public Point_Set<Exact>
  {
    stc_using_from(Point_Set, point);

    const point& pmin() const;
    point& pmin();
    const point& pmax() const;
    point& pmax();

  protected:
    Box();

  }; // end of oln::Box<Exact>


  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<Ps>& pts);

  template <typename B>
  std::ostream& operator<<(std::ostream& ostr, const Box<B>& b);


# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  unsigned
  Point_Set<Exact>::npoints() const
  {
    return exact(this)->impl_npoints();
  }

  template <typename Exact>
  bool
  Point_Set<Exact>::has(const typename Point_Set<Exact>::point& p) const
  {
    return exact(this)->impl_has(p);
  }

  template <typename Exact>
  const typename Point_Set<Exact>::box&
  Point_Set<Exact>::bbox() const
  {
    return exact(this)->impl_bbox();
  }

  template <typename Exact>
  Point_Set<Exact>::Point_Set()
  {
    this->check__();
  }

  template <typename Exact>
  void Point_Set<Exact>::check__() const
  {
    unsigned (Exact::*impl_npoints_adr)() const = & Exact::impl_npoints;
    impl_npoints_adr = 0;
    bool (Exact::*impl_has_adr)(const point& p) const = & Exact::impl_has;
    impl_has_adr = 0;
    const box& (Exact::*impl_bbox_adr)() const = & Exact::impl_bbox;
    impl_bbox_adr = 0;
  }

  template <typename Exact>
  Box<Exact>::Box()
  {
  }

  template <typename Exact>
  const typename Box<Exact>::point&
  Box<Exact>::pmin() const
  {
    return exact(this)->impl_pmin();
  }

  template <typename Exact>
  typename Box<Exact>::point&
  Box<Exact>::pmin()
  {
    return exact(this)->impl_pmin();
  }

  template <typename Exact>
  const typename Box<Exact>::point&
  Box<Exact>::pmax() const
  {
    return exact(this)->impl_pmax();
  }

  template <typename Exact>
  typename Box<Exact>::point&
  Box<Exact>::pmax()
  {
    return exact(this)->impl_pmax();
  }

  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<Ps>& pts)
  {
    ostr << "{ ";
    oln_piter(Ps) p(pts);
    for_all(p)
      ostr << p << ' '; // FIXME: Add ','?
    return ostr << " }";
  }

  template <typename B>
  std::ostream& operator<<(std::ostream& ostr, const Box<B>& b)
  {
    return ostr << "{ " << b.pmin() << " .. " << b.pmax() << " }";
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


// FIXME: Bad?
# include <oln/core/gen/pset_compare.hh>


#endif // ! OLN_CORE_CONCEPT_POINT_SET_HH
