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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_INTEGRE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_INTEGRE_HH

# include <oln/core/abstract/image_typeness.hh>


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


/*--------------------------.
| Typeness switch (cont.).  |
`--------------------------*/

namespace oln {

  
  /// Switch on on the value type.
  /// \{

  // ----------------------------------------------- //
  // Cases where the value type is an Integre type.  //
  // ----------------------------------------------- //

  // (The first cases are located in oln/core/abstract/image_typeness.hh).


  /// Binary case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 3> :
    public mlc::where_<
             mlc::or_list_< mlc_eq(value_type, ntg::bin),
			    ntg::eq_<ntg::int_u, 1, value_type>,
			    ntg::eq_<ntg::int_s, 1, value_type> > >
  {
    // Definition of the super class corresponding to this case.
    typedef stc::abstraction_as_type<abstract::binary_image> super_type;
    // Definition of the extended virtual type (same as the super
    // class in this case).
    typedef super_type image_typeness_type;
  };

  /// Grey-level case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 4> :
    public mlc::where_< mlc_is_a(value_type, ntg::real_value) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::grey_level_image_ is the conjunction of
    // abstract::grey_level_image and abstract::not_binary_image).
    typedef
      stc::abstraction_as_type<abstract::internal::grey_level_image_>
      super_type;
    // Definition of the extended virtual type.
    typedef stc::abstraction_as_type<abstract::grey_level_image>
      image_typeness_type;
  };

  /// Label case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 5> :
    public mlc::where_< mlc_is_a(value_type, ntg::enum_value) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::label_image_ is the conjunction of
    // abstract::label_image and abstract::not_binary_image).
    typedef stc::abstraction_as_type<abstract::internal::label_image_>
      super_type;
    // Definition of the extended virtual type.
    typedef stc::abstraction_as_type<abstract::label_image>
      image_typeness_type;
  };

  /// Color case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 6> :
    public mlc::where_< mlc_is_a(value_type, ntg::color_value) >
  {
    // Definition of the super class corresponding to this case
    // (abstract::color_image_ is the conjunction of
    // abstract::color_image and abstract::not_binary_image).
    typedef stc::abstraction_as_type<abstract::internal::color_image_>
      super_type;
    // Definition of the extended virtual type.
    typedef stc::abstraction_as_type<abstract::color_image>
      image_typeness_type;
  };

  /// \}

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_INTEGRE_HH
