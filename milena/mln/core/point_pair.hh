// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_POINT_PAIR_HH
# define MLN_CORE_POINT_PAIR_HH

/// \file mln/core/point_pair.hh
/// \brief Definition of the generic point_pair class mln::point_pair.

// FIXME: Might be renamed to something else, like ``segment''.

# include <utility>
# include <ostream>

# include <mln/core/concept/point.hh>


namespace mln
{

  /** \brief Generic point pair class.
      
       The sole parameter P is the type of the points of the pair.
      
       Pair points are mainly used to make a reference to a given edge
       in an line graph-based image.  */
  template <typename P>
  struct point_pair : public Point< point_pair<P> >
  {
    /// \var dim
    /// \brief Dimension of the space.
    /// \invariant dim > 0 
    enum { dim = P::dim };

    /// Mesh associated type.
    // FIXME: Dummy value.
    typedef mln_mesh(P) mesh;

    /// Dpoint associated type.
    // FIXME: Dummy value.
    typedef void dpoint;

    /// Coordinate associated type.
    typedef std::pair<typename P::coord, typename P::coord> coord;

    /// Constructors.
    /// \{
    /// Constructor without argument.
    point_pair();
    /// Constructor using two points.
    point_pair(const P& first, const P& second);
    /// Constructor using a pair of points.
    point_pair(const std::pair<P, P>& points);
    /// \}

    /// Accessors.
    /// \{

    /// \brief Read-only access to the \p i-th coordinate value.
    /// \param[in] i The coordinate index.
    /// \pre \p i < \c dim
    coord operator[](unsigned i) const;

    /* FIXME: Providing this accessor requires the point pair to be
       actually stored in this object, which both takes space and time
       (updates).  */

//     /// \brief Read-write access to the \p i-th coordinate value.
//     /// \param[in] i The coordinate index.
//     /// \pre \p i < \c dim
//     coord& operator[](unsigned i);

    /// Read-only access to the first point.
    P first() const;
    /// Read-write access to the first point.
    P& first();

    /// Read-only access to the second point.
    P second() const;
    /// Read-write access to the second point.
    P& second();
    /// \}

  protected:
    /// The pair of points.
    // FIXME: Should we use an ordpair_ instead of as std::pair?
    std::pair<P, P> points_;
  };

  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const point_pair<P>& p);


# ifndef MLN_INCLUDE_ONLY

  /*------------.
  | Accessors.  |
  `------------*/

  template <typename P>
  inline
  typename point_pair<P>::coord
  point_pair<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return std::make_pair(points_.first[i], points_.second[i]);
  }

//   template <typename P>
//   inline
//   typename point_pair<P>::coord&
//   point_pair<P>::operator[](unsigned i)
//   {
//     assert(i < dim);
//     return std::make_pair(points_.first[i], points_.second[i]);
//   }

  template <typename P>
  inline
  P
  point_pair<P>::first() const
  {
    return points_.first;
  }

  template <typename P>
  inline
  P&
  point_pair<P>::first()
  {
    return points_.first;
  }

  template <typename P>
  inline
  P
  point_pair<P>::second() const
  {
    return points_.second;
  }

  template <typename P>
  inline
  P&
  point_pair<P>::second()
  {
    return points_.second;
  }

  /*---------------.
  | Constructors.  |
  `---------------*/

  template <typename P>
  inline
  point_pair<P>::point_pair()
  {
  }

  template <typename P>
  inline
  point_pair<P>::point_pair(const P& first, const P& second)
    : points_(first, second)
  {
  }

  template <typename P>
  inline
  point_pair<P>::point_pair(const std::pair<P, P>& points)
    : points_(points)
  {
  }

  /*----------.
  | Display.  |
  `----------*/

  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const point_pair<P>& p)
  {
    return ostr << '(' << p.first() << " -- " << p.second() << ")";
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_POINT_PAIR_HH
