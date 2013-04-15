// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ALIAS_POINT2D_HH
# define MLN_CORE_ALIAS_POINT2D_HH

/// \file
///
/// Definition of the mln::point2d and mln::point2df aliases and of
/// their construction routines.

# include <mln/core/def/coord.hh>
# include <mln/core/def/coordf.hh>
# include <mln/core/point.hh>
// For site_const_impl and site_mutable_impl.
# include <mln/core/concept/site_proxy.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  /*!
    \class point2d
    \headerfile <>

    \brief Point defined on the 2D square grid with integer
    coordinates.
  */
  /// \cond ALIAS
  typedef point<mln::grid::square, mln::def::coord> point2d;
  /// \endcond


  /*!
    \class point2df
    \headerfile <>

    \brief Point defined on the 2D square grid with floating-point
    coordinates.
  */
  /// \cond ALIAS
  typedef point<mln::grid::square, mln::def::coordf> point2df;
  /// \endcond


  namespace internal
  {

    // Specialization.

    /// \internal
    template <typename C, typename E>
    struct subject_impl< const point<grid::square, C>, E >
    {
      typedef C coord;
      enum { dim = 2 };

      typedef const C& row_t;
      const C& row() const;

      typedef const C& col_t;
      const C& col() const;

      const C& operator[](unsigned i) const;
      const C& last_coord() const;
    private:
      const E& exact_() const;
    };


    // Specialization for point<M,C>.

    /// \internal
    template <typename C, typename E>
    struct subject_impl<       point<grid::square, C>, E > :
           subject_impl< const point<grid::square, C>, E >
    {
    private:
      typedef subject_impl< const point<grid::square, C>, E > super_;
      E& exact_();
    public:
      using super_::row;
      C& row();
      using super_::col;
      C& col();
      using super_::operator[];
      C& operator[](unsigned i);
    };


# ifndef MLN_INCLUDE_ONLY

    // subject_impl

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::square, C>, E >::row() const
    {
      return exact_().get_subject().row();
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::square, C>, E >::col() const
    {
      return exact_().get_subject().col();
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::square, C>, E >::operator[](unsigned i) const
    {
      mln_precondition(i < 2);
      return exact_().get_subject()[i];
    }

    template <typename C, typename E>
    inline
    const C&
    subject_impl< const point<grid::square, C>, E >::last_coord() const
    {
      return this->col();
    }

    template <typename C, typename E>
    inline
    const E&
    subject_impl< const point<grid::square, C>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    // subject_impl

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::square, C>, E >::row()
    {
      return exact_().get_subject().row();
    }

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::square, C>, E >::col()
    {
      return exact_().get_subject().col();
    }

    template <typename C, typename E>
    inline
    C&
    subject_impl< point<grid::square, C>, E >::operator[](unsigned i)
    {
      mln_precondition(i < 2);
      return exact_().get_subject()[i];
    }

    template <typename C, typename E>
    inline
    E&
    subject_impl< point<grid::square, C>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal


} // end of namespace mln


# include <mln/core/alias/dpoint2d.hh>


#endif // ! MLN_CORE_ALIAS_POINT2D_HH
