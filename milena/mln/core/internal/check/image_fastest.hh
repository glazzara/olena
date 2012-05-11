// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_CORE_INTERNAL_CHECK_IMAGE_FASTEST_HH
# define MLN_CORE_INTERNAL_CHECK_IMAGE_FASTEST_HH

/// \file
///
/// \brief Class that statically checks the interface of fastest
/// images.
///
/// \todo Check and convert p in offset_of_point towards E::psite.

# include <mln/metal/bool.hh>
# include <mln/core/macros.hh>
# include <mln/core/trait/pixter.hh>
# include <mln/core/trait/qlf_value.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
  {

    namespace check
    {

      /// FIXME
      template < typename E, typename B = metal::true_ >
      struct image_fastest_
      {

	/// \brief Give the offset of the site \p p.
	///
	/// \param[in] p A site.
	///
	/// \warning This method is final.
	///
	/// \pre The image has to be initialized and to own the point \p p.
	/// \post p == point_at_offset(result)
	///
	template <typename P>
	unsigned
	offset_of_point(const P& p) const;

      protected:
	image_fastest_();
      };


      template <typename E>
      struct image_fastest_< E, metal::false_ >
      {
	// Nothing.
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename E, typename B>
      inline
      image_fastest_<E,B>::image_fastest_()
      {
	typedef mln_site(E)        site;
	typedef mln_psite(E)      psite;
	typedef mln_delta(psite) dpsite;


	typedef mln_fwd_pixter(E) fwd_pixter;
	typedef mln_bkd_pixter(E) bkd_pixter;

	int (E::*m1)(const dpsite&) const = & E::delta_offset;
	(void) m1;
	site (E::*m2)(unsigned) const = & E::point_at_offset;
	(void) m2;
	unsigned (E::*m3)() const = & E::border;
	(void) m3;

	typedef mln_value(E) value;

	mln_qlf_value(E)* (E::*m4)() = & E::buffer;
	(void) m4;
	const value* (E::*m5)() const = & E::buffer;
	(void) m5;

	typedef mln_rvalue(E) rvalue;
	typedef mln_lvalue(E) lvalue;

	rvalue (E::*m6)(unsigned) const = & E::element;
	(void) m6;
	lvalue (E::*m7)(unsigned) = & E::element;
	(void) m7;

	unsigned (E::*m8)() const = & E::nelements;
	(void) m8;

#  if (defined(__GNUC__) && __GNUC__ >= 3) && !defined(__INTEL_COMPILER)
	unsigned (E::*m9)(const psite& p) const = & E::offset_of_point;
	(void) m9;
#  endif

	// FIXME: how to check that qixter are defined when W is unknown!
      }

      template <typename E, typename B>
      template <typename P>
      inline
      unsigned
      image_fastest_<E,B>::offset_of_point(const P& p) const
      {
	const E* this_ = & internal::force_exact<E>(*this);
	mln_precondition(this_->is_valid());
	mln_precondition(this_->has(p));

	unsigned i = & this_->operator()(p) - this_->buffer();
	mln_postcondition(p == this_->point_at_offset(i));
	return i;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::internal::check

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_CHECK_IMAGE_FASTEST_HH
