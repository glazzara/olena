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

#ifndef MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH
# define MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH

/*! \file mln/core/internal/image_adaptor.hh
 *
 * \brief Definition of a base class for image adaptors.
 */

# include <mln/core/internal/image_base.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A base class for image adaptors.
     *
     * \internal
     */
    template < typename I,
	       typename E,
	       typename S = mln_pset(I) >
    struct image_adaptor_ : public internal::image_base_< S, E >
    {

      /// Point_Site associated type.
      typedef mln_psite(S) psite;

      /// Point_Set associated type.
      typedef S pset;

      /// Value_Set associated type.
      typedef mln_vset(I) vset;

      /// Value associated type.
      typedef mln_value(I)   value;

      /// Return type of read-only access.
      typedef mln_rvalue(I) rvalue;

      /// Return type of read-write access.
      typedef typename internal::morpher_lvalue_<I>::ret lvalue;


      /// Test if this image has been initialized.
      bool has_data() const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give the definition domain.
      const mln_pset(I)& domain() const;

      /// Give the set of values.
      const vset& values() const;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const psite& p);

      /// Convertion to the underlying (adapted) image.
      operator I() const;

    protected:
      I& adaptee_;

      /// Constructor from an \p adaptee image.
      image_adaptor_(I& adaptee);
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E, typename S>
    image_adaptor_<I,E,S>::image_adaptor_(I& adaptee)
      : adaptee_(adaptee)
    {
    }

    template <typename I, typename E, typename S>
    bool image_adaptor_<I,E,S>::has_data() const
    {
      return adaptee_.has_data();
    }

    template <typename I, typename E, typename S>
    bool image_adaptor_<I,E,S>::owns_(const psite& p) const
    {
      mln_precondition(exact(this)->has_data());
      return adaptee_.owns_(p);
    }

    template <typename I, typename E, typename S>
    const mln_pset(I)&
    image_adaptor_<I,E,S>::domain() const
    {
      mln_precondition(exact(this)->has_data());
      return adaptee_.domain();
    }

    template <typename I, typename E, typename S>
    const mln_vset(I)&
    image_adaptor_<I,E,S>::values() const
    {
      return adaptee_.values();
    }

    template <typename I, typename E, typename S>
    typename image_adaptor_<I,E,S>::rvalue
    image_adaptor_<I,E,S>::operator()(const psite& p) const
    {
      mln_precondition(exact(this)->owns_(p));
      return adaptee_(p);
    }

    template <typename I, typename E, typename S>
    typename image_adaptor_<I,E,S>::lvalue
    image_adaptor_<I,E,S>::operator()(const psite& p)
    {
      mln_precondition(exact(this)->owns_(p));
      return adaptee_(p);
    }

    template <typename I, typename E, typename S>
    image_adaptor_<I,E,S>::operator I() const
    {
      mln_precondition(exact(this)->has_data());
      return adaptee_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH
