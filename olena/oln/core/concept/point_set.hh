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

# include <ostream>
# include <oln/core/concept/point.hh>


namespace oln
{


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

  }; // end of oln::Point_Set<Exact>


  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<Ps>& pts);


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
  }


  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<Ps>& pts)
  {
    ostr << "{ ";
    typename Ps::piter p(pts);
    for_all(p)
      ostr << p << ' '; // FIXME: Add ','?
    return ostr << " }";
  }


# endif


} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_POINT_SET_HH
