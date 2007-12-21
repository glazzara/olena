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

#ifndef MLN_CORE_CONCEPT_IMAGE_HH
# define MLN_CORE_CONCEPT_IMAGE_HH

/*! \file mln/core/concept/image.hh
 * \brief Definition of the concept of mln::Image.
 */

# include <mln/core/concept/point_set.hh>
# include <mln/core/concept/mesh.hh>

# include <mln/core/trait/all.hh> // FIXME: Move out of core!
# include <mln/core/macros.hh>
# include <mln/trait/concrete.hh> // FIXME: Should be in all.hh!
# include <mln/trait/images.hh>

# include <mln/metal/is_a.hh>
# include <mln/metal/equal.hh>

# include <mln/tag/init.hh>



namespace mln
{


  // Fwd decl.
  template <typename E> struct Image;

  // Image category flag type.
  template <>
  struct Image<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation of image classes.
   *
   * \see mln::doc::Image for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Image : public Object<E>
  {
    typedef Image<void> category;

    /*
      // to be provided in concrete image classes:

      typedef mesh;

      typedef value;
      typedef rvalue;
      typedef lvalue;
      typedef vset;

      const vset& values() const;

      bool owns_(const psite& p) const;
      const pset& domain() const;

      rvalue operator()(const psite& p) const;
      lvalue operator()(const psite& p);

      typedef skeleton;


      // provided by internal::image_base_:

      typedef pset;
      typedef point;
      typedef psite;

      typedef coord;
      typedef dpoint;

      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      const box_<point>& bbox() const;
      std::size_t npoints() const;

      bool has_data() const;
     */

  protected:
    Image();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Image<E>::Image()
  {
    // provided by internal::image_base_:

    typedef mln_pset(E)  pset;
    typedef mln_point(E) point;
    typedef mln_psite(E) psite;

    typedef mln_coord(E)  coord;
    typedef mln_dpoint(E) dpoint;

    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    const box_<point>& (E::*m2)() const = & E::bbox;
    m2 = 0;

    // to be provided in concrete image classes:

    typedef mln_mesh(E) mesh;
    metal::is_a<mesh, Mesh>::check(); // FIXME: Add other checks.

    typedef mln_value(E)  value;
    typedef mln_rvalue(E) rvalue;
    typedef mln_lvalue(E) lvalue;

    typedef typename E::skeleton skeleton;

    bool (E::*m3)() const = & E::has_data;
    m3 = 0;
    bool (E::*m4)(const psite& p) const = & E::owns_;
    m4 = 0;
    const pset& (E::*m5)() const = & E::domain;
    m5 = 0;

    rvalue (E::*m6)(const psite& p) const = & E::operator();
    m6 = 0;
    lvalue (E::*m7)(const psite& p) = & E::operator();
    m7 = 0;

    typedef mln_vset(E) vset;
    const vset& (E::*m8)() const = & E::values;
    m8 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# include <mln/core/initialize.hh>

#endif // ! MLN_CORE_CONCEPT_IMAGE_HH
