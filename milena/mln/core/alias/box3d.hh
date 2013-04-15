// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_ALIAS_BOX3D_HH
# define MLN_CORE_ALIAS_BOX3D_HH

/// \file
///
/// Definition of the mln::box3d alias and of construction
/// routines.

# include <mln/core/site_set/box.hh>
# include <mln/core/alias/point3d.hh>


namespace mln
{

  /*!
    \class mln::box3d
    \headerfile <>

    \brief A box defined on the 3D square grid with integer
    coordinates.

    This class is a site set of mln::point3d. It represents a rectangle of
    mln::point3d which is usually used as domain for a mln::image3d.

    mln::box3d is a typedef of box<mln::point3d>.

    Iteration over the mln::point3d of a mln::box3d relies on p-iterators
    and it can be performed using the following code:

    \code
    box3d b(3,3,3);
    mln_piter(box3d) p(b);
    for_all(p)
      std::cout << p << std::endl;
    \endcode

    Common sites can be retrieved through pmin(), pmax() and pcenter().
    \verbatim
      pmin
         X------------.
        /|           /|
       .-|---------./ |
       | |    X    |  |
       | .---------|--.-------->
       |/  pcenter | /
       |           |/
       ------------X
      /           pmax
     /
    \/
    \endverbatim


    \sa mln::make::box3d.

    \ingroup modsitesetbasic
  */
  /// \cond ALIAS
  typedef box<point3d> box3d;
  /// \endcond

} // end of namespace mln


# include <mln/make/box3d.hh>


#endif // ! MLN_CORE_ALIAS_BOX3D_HH
