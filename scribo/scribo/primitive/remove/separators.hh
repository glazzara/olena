// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_REMOVE_SEPARATORS_HH
# define SCRIBO_PRIMITIVE_REMOVE_SEPARATORS_HH

/// \file
///
/// Remove separators in a binary image.

# include <mln/core/concept/image.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/data/fill.hh>

# include <mln/border/fill.hh>
# include <mln/border/resize.hh>


namespace scribo
{

  namespace primitive
  {

    namespace remove
    {

      using namespace mln;

      /// \brief Remove separators in a binary image.
      /*!
       *
       * \param[in] input A binary image. 'True' for objects, 'False'
       *                  for the background.
       * \param[in] separators A binary image of separators.
       *
       * \return \p input without separators.
       *
       * \sa primitive::extract::separators
       */
      template <typename I>
      mln_concrete(I)
      separators(const Image<I>& input, const Image<I>& separators);



# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      separators(const Image<I>& input_, const Image<I>& separators_)
      {
	trace::entering("scribo::primitive::remove::separators");

	const I& input = exact(input_);
	const I& separators = exact(separators_);
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());
	mln_precondition(separators.is_valid());

	mln_concrete(I) output = duplicate(input);

	border::resize(separators, output.border());
	border::fill(separators, 0); // FIXME: we should rewrite image
				     // browsing below instead.

	typedef const mln_value(I)* sep_ptr_t;
	sep_ptr_t
	  sep_ptr = separators.buffer(),
	  end_ptr = sep_ptr + input.nelements();
	mln_value(I) *out_ptr = output.buffer();

	for (; sep_ptr < end_ptr; ++out_ptr)
	  if (*sep_ptr++)
	    *out_ptr = false;

	trace::exiting("scribo::primitive::remove::separators");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::remove

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_REMOVE_SEPARATORS_HH
