// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_ALIAS_POINT3D_HH
# define MLN_CORE_ALIAS_POINT3D_HH

/// \file
///
/// Definition of the mln::point3d and mln::point3df aliases and of
/// their construction routines.

# include <mln/core/def/coord.hh>
# include <mln/core/def/coordf.hh>
# include <mln/core/point.hh>
// For site_const_impl and site_mutable_impl:
# include <mln/core/concept/site_proxy.hh>
# include <mln/core/internal/force_exact.hh>

namespace mln
{

  /// Type alias for a point defined on the 3D square grid with
  /// integer coordinates.
  typedef point<grid::cube, def::coord> point3d;

  /// Type alias for a point defined on the 3D square grid with
  /// floating-point coordinates.
  typedef point<grid::cube, def::coordf> point3df;

  namespace internal
  {

    // Specialization.

    template <typename C, typename E>
    struct subject_impl< const point<grid::cube, C>, E >
      : subject_point_impl< point<grid::cube, C>, E >
    {
      typedef C coord;
      enum { dim = 3 };

      typedef const C& row_t;
      const C& row() const;

      typedef const C& col_t;
      const C& col() const;

      typedef const C& sli_t;
      const C& sli() const;

      const C& operator[](unsigned i) const;
      const C& last_coord() const;
    private:
      const E& exact_() const;
    };


    // Specialization for point<M,C>.

    template <typename C, typename E>
    struct subject_impl<       point<grid::cube, C>, E > :
           subject_impl< const point<grid::cube, C>, E >
    {
    private:
      typedef subject_impl< const point<grid::cube, C>, E > super_;
      E& exact_();
    public:

      using super_::row;
      C& row();

      using super_::col;
      C& col();

      using super_::sli;
      C& sli();

      using super_::operator[];
      C& operator[](unsigned i);
    };



# ifndef MLN_INCLUDE_ONLY

    // subject_impl

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::cube, C>, E >::row() const
    {
      return exact_().get_subject().row();
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::cube, C>, E >::col() const
    {
      return exact_().get_subject().col();
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::cube, C>, E >::sli() const
    {
      return exact_().get_subject().sli();
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::cube, C>, E >::operator[](unsigned i) const
    {
      mln_precondition(i < 3);
      return exact_().get_subject()[i];
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::cube, C>, E >::last_coord() const
    {
      return this->col();
    }

    template <typename C, typename E>
    inline
    const E&
    subject_impl< const point<grid::cube, C>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    // subject_impl

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::cube, C>, E >::row()
    {
      return exact_().get_subject().row();
    }

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::cube, C>, E >::col()
    {
      return exact_().get_subject().col();
    }

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::cube, C>, E >::sli()
    {
      return exact_().get_subject().sli();
    }

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::cube, C>, E >::operator[](unsigned i)
    {
      mln_precondition(i < 3);
      return exact_().get_subject()[i];
    }

    template <typename C, typename E>
    inline
    E&
    subject_impl< point<grid::cube, C>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal


} // end of namespace mln


# include <mln/core/alias/dpoint3d.hh>


#endif // ! MLN_CORE_ALIAS_POINT3D_HH
