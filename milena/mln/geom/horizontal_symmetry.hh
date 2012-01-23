// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// Performs a horizontal symmetry.

#ifndef MLN_GEOM_HORIZONTAL_SYMMETRY_HH
# define MLN_GEOM_HORIZONTAL_SYMMETRY_HH

# include <mln/core/concept/image.hh>
# include <mln/core/box_runend_piter.hh>
# include <mln/core/box_runstart_piter.hh>

namespace mln
{

  namespace geom
  {

    /*! \brief Perfoms a horizontal symmetry.

      \ingroup mlngeom
    */
    template <typename I>
    mln_concrete(I)
    horizontal_symmetry(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    // Implementations

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	mln_concrete(I)
	horizontal_symmetry(const Image<I>& input_)
	{
	  const I& input = exact(input_);
	  mln_precondition(input.is_valid());

	  mln_concrete(I) output(input.domain());

	  typedef mln_site(I) P;
	  box_runstart_piter<P> pi(input.domain());
	  box_runend_piter<P> po(output.domain());

	  unsigned ncols = input.ncols();

	  for_all_2(pi, po)
	  {
	    mln_site(I) idi = pi, ido = po;
	    ido[1] -= ncols - 1;
	    for (unsigned n = 0; n < ncols; ++n, ++idi[1], ++ido[1])
	      output(ido) = input(idi);
	  }

	  return output;
	}

      } // end of namespace mln::geom::impl::generic


      template <typename I>
      mln_concrete(I)
      horizontal_symmetry_fastest(const Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	mln_concrete(I) output(input.domain());

	typedef mln_site(I) P;
	box_runstart_piter<P> pi(input.domain());
	box_runend_piter<P> po(output.domain());

	unsigned ncols = input.ncols();

	typedef mln_value(I)* ptr_t;

	for_all_2(pi, po)
	{
	  const mln_value(I)* ptr_in  = & input(pi);
	  ptr_t ptr_out = (& output(po)) - ncols + 1;

	  for (unsigned n = 0; n < ncols; ++n)
	    *ptr_out++ = *ptr_in++;
	}

	return output;
      }


    } // end of namespace mln::geom::impl



    // Dispatch

    namespace internal
    {

      template <typename I>
      mln_concrete(I)
      horizontal_symmetry_dispatch(
	trait::image::value_alignment::any,
	trait::image::value_storage::any,
	trait::image::value_access::any,
	const Image<I>& input)
      {
	return impl::generic::horizontal_symmetry(input);
      }


      template <typename I>
      mln_concrete(I)
      horizontal_symmetry_dispatch(
	trait::image::value_alignment::with_grid,
	trait::image::value_storage::one_block,
	trait::image::value_access::direct,
	const Image<I>& input)
      {
	return impl::horizontal_symmetry_fastest(input);
      }


      template <typename I>
      mln_concrete(I)
      horizontal_symmetry_dispatch(const Image<I>& input)
      {
	return horizontal_symmetry_dispatch(
	  mln_trait_image_value_alignment(I)(),
	  mln_trait_image_value_storage(I)(),
	  mln_trait_image_value_access(I)(),
	  input);
      }

    } // end of namespace mln::geom::internal



    // Facade

    template <typename I>
    mln_concrete(I)
    horizontal_symmetry(const Image<I>& input_)
    {
      trace::entering("geom::horizontal_symmetry");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      mln_concrete(I) output = internal::horizontal_symmetry_dispatch(input);

      trace::exiting("geom::horizontal_symmetry");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_HORIZONTAL_SYMMETRY_HH
