// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_ROUTINE_PRIMARY_HH
# define MLN_CORE_ROUTINE_PRIMARY_HH

/// \file mln/core/routine/primary.hh
///
/// Get the primary image behind any image.
///
/// \todo We also need to get the extension image to handle border
/// routines.

# include <mln/core/concept/image.hh>


namespace mln
{

  // Forward declaration.
  namespace internal
  {
    template <typename I> struct primary_type;
  }


  /// FIXME: Doc!
  template <typename I>
  const typename internal::primary_type<I>::ret&
  primary(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {

    // Primary_type.

    template <typename I> struct primary_type;

    template <typename I, typename C>
    struct primary_type_helper
    {
      typedef typename I::delegatee D;
      typedef typename primary_type<D>::ret ret;
    };

    template <typename I>
    struct primary_type_helper< I, mln::trait::image::category::primary >
    {
      typedef I ret;
    };

    template <typename I>
    struct primary_type
    {
      typedef mln_trait_image_category(I) Cat;
      typedef typename primary_type_helper<I, Cat>::ret ret;
    };


    // Routine.

    template <typename I>
    const typename internal::primary_type<I>::ret&
    primary_(const Image<I>& input); // Forward declaration.

    template <typename I>
    inline
    const typename internal::primary_type<I>::ret&
    primary_(trait::image::category::primary,
	     const Image<I>& input)
    {
      return exact(input);
    }

    template <typename I>
    inline
    const typename internal::primary_type<I>::ret&
    primary_(trait::image::category::morpher,
	     const Image<I>& input)
    {
      return primary_(exact(input).unmorph_());
    }

    template <typename I>
    inline
    const typename internal::primary_type<I>::ret&
    primary_(const Image<I>& input)
    {
      return primary_(mln_trait_image_category(I)(),
		      input);
    }

  } // end of namespace mln::internal



    // Facade.

  template <typename I>
  inline
  const typename internal::primary_type<I>::ret&
  primary(const Image<I>& input)
  {
    mln_precondition(exact(input).is_valid());
    return internal::primary_(input);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_PRIMARY_HH
