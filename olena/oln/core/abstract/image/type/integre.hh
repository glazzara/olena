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


// FIXME: Move this file into Integre? Or in a another project,
// serving as a glue between Integre and Olena?

#ifndef OLN_CORE_ABSTRACT_IMAGE_TYPE_INTEGRE_HH
# define OLN_CORE_ABSTRACT_IMAGE_TYPE_INTEGRE_HH

# include <oln/core/abstract/image/type/hierarchy.hh>


// Forward declarations.
/* FIXME: These are Integre values; should'nt we let Integre add the
   necessary cases itself?  */
namespace ntg
{
  class bin;
  template <unsigned nbits, typename behavior> class int_u;
  template <unsigned nbits, typename behavior> class int_s;
  template <typename E> class color_value;
  template <typename E> class enum_value;
  template <typename E> class real_value;

    /** \brief ``eq'' operator dedicated to ntg::int_u<nbits, behavior> and
	ntg::int_s<nbits, behavior>, with nbits a known unsigned (e.g.,
	int_u<1, behavior>).

	Checking whether value_type is a ntg::int_u<1, behavior>
	(behavior being a template parameter) is a bit tricky: we
	can't use mlc_eq() since ntg::int_u<1, B> is not a (complete)
	type, nor mlc_is_a(), since it is a partially-valued template.
	Hence this dedicated approach, used for ntg::int_u and
	ntg::int_s.

	(Olena proto-1.0 used to mimic mlc_is_a with its own
	machinery, for all kind of value type, which is redundant with
	mlc_is_a).
	\{ */
    template< template <unsigned, class> class T, unsigned nbits, typename U >
    struct eq_ : public mlc::bexpr_<false>
    {
    };

    template< template <unsigned, class> class T, unsigned nbits,
	      typename behavior >
    struct eq_< T, nbits, T<nbits, behavior> > : public mlc::bexpr_<true>
    {
    };
    /** \} */

} // end of namespace ntg


/*----------------------.
| Type switch (cont.).  |
`----------------------*/

namespace oln {

  
  /// Switch on on the value type.
  /// \{

  // ----------------------------------------------- //
  // Cases where the value type is an Integre type.  //
  // ----------------------------------------------- //

  // (The first cases are located in oln/core/abstract/image_type.hh).


  /// Binary case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 3 > :
    where_< mlc::or_list_< mlc::eq_<oln_type_of(E, value), ntg::bin>,
			   ntg::eq_<ntg::int_u, 1, oln_type_of(E, value)>,
			   ntg::eq_<ntg::int_s, 1, oln_type_of(E, value)> > >
  {
    // Definition of the super class corresponding to this case.
    typedef abstract::binary_image<E> ret;
  };

  /// Grey-level case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 4 > :
    where_< mlc_is_a( oln_type_of(E, value), ntg::real_value ) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::grey_level_image_ is the conjunction of
    // abstract::grey_level_image and abstract::not_binary_image).
    typedef abstract::grey_level_image<E> ret;
  };

  /// Label case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 5 > :
    where_< mlc_is_a( oln_type_of(E, value), ntg::enum_value ) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::label_image_ is the conjunction of
    // abstract::label_image and abstract::not_binary_image).
    typedef abstract::label_image<E> ret;
  };

  /// Color case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 6 > :
    where_< mlc_is_a( oln_type_of(E, value), ntg::color_value ) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::color_image_ is the conjunction of
    // abstract::color_image and abstract::not_binary_image).
    typedef abstract::color_image<E> ret;
  };

  /// \}

} // end of namespace oln

#endif // ! OLN_CORE_ABSTRACT_IMAGE_TYPE_INTEGRE_HH
