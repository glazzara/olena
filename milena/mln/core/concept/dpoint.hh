// Copyright (C) 2007, 2009, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_DPOINT_HH
# define MLN_CORE_CONCEPT_DPOINT_HH

/*! \file
 * \brief Definition of the concept of mln::Dpoint.
 */

# include <mln/core/concept/delta_point_site.hh>
# include <mln/core/grids.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Dpoint;


  /// \cond INTERNAL_API
  /// Dpoint category flag type.
  template <>
  struct Dpoint<void>
  {
    typedef Delta_Point_Site<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation of delta-point classes.

    A delta-point is a vector defined by a couple of points.

    Given two points, A and B, the vector AB is mapped into the
    delta-point D = AB.  Practically one can write: D = B - A.

    \ingroup modconcepts
  */
  template <typename E>
  struct Dpoint : public Delta_Point_Site<E>
  {
    typedef Dpoint<void> category;

    typedef E dpoint;

    /// It is a Dpoint so it returns itself.
    const E& to_dpoint() const;

  protected:
    Dpoint();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Dpoint<E>::Dpoint()
  {
  }

  template <typename E>
  inline
  const E&
  Dpoint<E>::to_dpoint() const
  {
    return exact(*this);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_DPOINT_HH
