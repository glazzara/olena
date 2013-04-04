// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_ALIAS_VEC3D_HH
# define MLN_CORE_ALIAS_VEC3D_HH

/// \file
///
/// Definition of the mln::vec3d* alias


# include <mln/algebra/vec.hh>

namespace mln
{

  /*!
    \class vec3d_f
    \headerfile <>

    \brief 3D vector with float coordinates.

    \ingroup mlnalgebratypes
  */
  /// \cond ALIAS
  typedef algebra::vec<3u,float> vec3d_f;
  /// \endcond

  /*!
    \class vec3d_f
    \headerfile <>

    \brief 3D vector with double coordinates.

    \ingroup mlnalgebratypes
  */
  /// \cond ALIAS
  typedef algebra::vec<3u,double> vec3d_d;
  /// \endcond


} // end of namespace mln


# include <mln/make/box3d.hh>


#endif // ! MLN_CORE_ALIAS_VEC3D_HH
