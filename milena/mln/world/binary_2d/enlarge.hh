// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef MLN_WORLD_BINARY_2D_ENLARGE_HH
# define MLN_WORLD_BINARY_2D_ENLARGE_HH

/// \file
///
/// \brief Enlarge 2^n times a binary image.
///
/// \todo Make it much more generic.


# include <iostream>

# include <mln/core/image/image2d.hh>
# include <mln/core/routine/initialize.hh>

# include <mln/value/int_u8.hh>
# include <mln/fun/p2v/ternary.hh>
# include <mln/fun/v2b/threshold.hh>

# include <mln/data/transform.hh>

# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/pw/value.hh>
# include <mln/opt/at.hh>

# include <mln/geom/min_row.hh>
# include <mln/geom/min_col.hh>

# include <mln/core/routine/duplicate.hh>


namespace mln
{

  namespace world
  {

    namespace binary_2d
    {

      /// Enlarge a binary image 2^n times
      ///
      /// \param[in] input A binary image.
      /// \param[in] n	   The power of 2 setting up the enlargement.
      ///
      /// \return A binary image with a domain 2^\p n times larger.
      //
      template <typename I>
      mln_concrete(I)
      enlarge(const Image<I>& input, unsigned n);


# ifndef MLN_INCLUDE_ONLY


      // Internal routines.

      namespace internal
      {

	inline
	float
	val(bool b)
	{
	  return b ? 1 : 0;
	}

	inline
	int
	do_threshold(float value)
	{
	  return static_cast<int>(255.f * value);
	}

      } // end of namespace mln::world::binary_2d




      // Implementation.

      namespace impl
      {

	///FIXME: works only on 2d images.
	inline
	image2d<value::int_u8>
	enlargex2(const image2d<bool>& input)
	{
	  using value::int_u8;

	  mln_precondition(input.is_valid());

	  def::coord
	    mrow = geom::min_row(input),
	    mcol = geom::min_col(input);

	  image2d<int_u8> output(make::box2d(mrow, mcol,
					     mrow + 2 * input.nrows() - 1,
					     mcol + 2 * input.ncols() - 1));
	  float value;

	  // row 0
	  opt::at(output, mrow, mcol) = internal::do_threshold(opt::at(input, mrow, mcol));

	  for (unsigned col = 2; col < output.ncols(); col += 2)
	  {
	    value = internal::val(opt::at(input, mrow, mcol + col / 2));
	    value += internal::val(opt::at(input, mrow, mcol + col / 2 - 1));
	    opt::at(output, mrow, mcol + col) = internal::do_threshold(value / 2);
	  }

	  for (unsigned col = 1; col < output.ncols(); col += 2)
	    opt::at(output, mrow, mcol + col)
	      = internal::do_threshold(opt::at(input, mrow, mcol + col / 2));

	  // col 0

	  for (unsigned row = 2; row < output.nrows(); row += 2)
	  {
	    value = internal::val(opt::at(input, mrow + row / 2, mcol));
	    value += internal::val(opt::at(input, mrow + row / 2 - 1, mcol));
	    opt::at(output, mrow + row, mcol) = internal::do_threshold(value / 2);
	  }

	  for (unsigned row = 1; row < output.nrows(); row += 2)
	    opt::at(output, mrow + row, mcol)
	      = internal::do_threshold(opt::at(input, mrow + row / 2, mcol));

	  // others

	  for (unsigned row = 2; row < output.nrows(); row += 2)
	  {
	    for (unsigned col = 2; col < output.ncols(); col += 2)
	    {
	      value = internal::val(opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += internal::val(opt::at(input, mrow + row / 2 - 1, mcol + col / 2));
	      value += internal::val(opt::at(input, mrow + row / 2, mcol + col / 2 - 1));
	      value += internal::val(opt::at(input, mrow + row / 2 - 1, mcol + col / 2 - 1));
	      opt::at(output, mrow + row, mcol + col)
		= internal::do_threshold(value / 4);
	    }
	    for (unsigned col = 1; col < output.ncols(); col += 2)
	    {
	      value = internal::val(opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += internal::val(opt::at(input, mrow + row / 2 - 1, mcol + col / 2));
	      opt::at(output, mrow + row, mcol + col) = internal::do_threshold(value / 2);
	    }
	  }

	  for (unsigned row = 1; row < output.nrows(); row += 2)
	  {
	    for (unsigned col = 2; col < output.ncols(); col += 2)
	    {
	      value = internal::val(opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += internal::val(opt::at(input, mrow + row / 2, mcol + col / 2 - 1));
	      opt::at(output, mrow + row, mcol + col) = internal::do_threshold(value / 2);
	    }
	    for (unsigned col = 1; col < output.ncols(); col += 2)
	      opt::at(output, mrow + row, mcol + col)
		= internal::do_threshold(opt::at(input, mrow + row / 2, mcol + col / 2));
	  }

	  return output;
	}



	inline
        image2d<value::int_u8>
        enlargex2(const image2d<value::int_u8>& input)
        {
	  using value::int_u8;

	  unsigned
	    mrow = geom::min_row(input),
		 mcol = geom::min_col(input);

	  image2d<int_u8> output(make::box2d(mrow, mcol,
					     mrow + 2 * input.nrows() - 1,
					     mcol + 2 * input.ncols() - 1));
	  unsigned value;

	  // row 0
	  opt::at(output, mrow, mcol) = (opt::at(input, mrow, mcol));

	  for (unsigned col = 2; col < output.ncols(); col += 2)
	  {
	    value = (opt::at(input, mrow, mcol + col / 2));
	    value += (opt::at(input, mrow, mcol + col / 2 - 1));
	    opt::at(output, mrow, mcol + col) = (value / 2);
	  }

	  for (unsigned col = 1; col < output.ncols(); col += 2)
	    opt::at(output, mrow, mcol + col) = (opt::at(input, mrow, mcol + col / 2));

	  // col 0

	  for (unsigned row = 2; row < output.nrows(); row += 2)
	  {
	    value = (opt::at(input, mrow + row / 2, mcol));
	    value += (opt::at(input, mrow + row / 2 - 1, mcol));
	    opt::at(output, mrow + row, mcol) = (value / 2);
	  }

	  for (unsigned row = 1; row < output.nrows(); row += 2)
	    opt::at(output, mrow + row, mcol) = (opt::at(input, mrow + row / 2, mcol));

	  // others

	  for (unsigned row = 2; row < output.nrows(); row += 2)
	  {
	    for (unsigned col = 2; col < output.ncols(); col += 2)
	    {
	      value = (opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += (opt::at(input, mrow + row / 2 - 1, mcol + col / 2));
	      value += (opt::at(input, mrow + row / 2, mcol + col / 2 - 1));
	      value += (opt::at(input, mrow + row / 2 - 1, mcol + col / 2 - 1));
	      opt::at(output, mrow + row, mcol + col) = ((unsigned(value)+2) / 4);
	    }
	    for (unsigned col = 1; col < output.ncols(); col += 2)
	    {
	      value = (opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += (opt::at(input, mrow + row / 2 - 1, mcol + col / 2));
	      opt::at(output, mrow + row, mcol + col) = (value / 2);
	    }
	  }

	  for (unsigned row = 1; row < output.nrows(); row += 2)
	  {
	    for (unsigned col = 2; col < output.ncols(); col += 2)
	    {
	      value = (opt::at(input, mrow + row / 2, mcol + col / 2));
	      value += (opt::at(input, mrow + row / 2, mcol + col / 2 - 1));
	      opt::at(output, mrow + row, mcol + col) = (value / 2);
	    }
	    for (unsigned col = 1; col < output.ncols(); col += 2)
	      opt::at(output, mrow + row, mcol + col)
		= (opt::at(input, mrow + row / 2, mcol + col / 2));
	  }

	  return output;
	}


	template <typename I>
	inline
	mln_ch_value(I,value::int_u8)
	do_enlarge_gl(const I& input, unsigned n)
	{
	  using value::int_u8;

	  mln_ch_value(I,int_u8) output = enlargex2(input);

	  while (--n)
	    output = enlargex2(output);

	  return output;
	}


	template <typename I>
	inline
	mln_concrete(I)
	do_enlarge_bool(const I& input, unsigned n)
	{
	  mln_ch_value(I,value::int_u8) tmp = do_enlarge_gl(input, n);
	  I output
	    = data::transform(tmp, fun::v2b::threshold<value::int_u8>(150));
	  return output;
	}


      } // end of namespace mln::world::binary_2d::impl




      // Dispatch

      namespace internal
      {

	template<typename I>
	inline
	mln_concrete(I)
	enlarge_dispatch(const I& input, const bool&, unsigned n)
	{
	  return impl::do_enlarge_bool(input, n);
	}

	template<typename I>
	inline
	mln_concrete(I)
	enlarge_dispatch(const I& input, const value::int_u8&, unsigned n)
	{
	  return impl::do_enlarge_gl(input, n);
	}

	template<typename I>
	inline
	mln_concrete(I)
	enlarge_dispatch(const I& input, const mln_value(I)&, unsigned n)
	{
	  (void) input;
	  (void) n;

	  mlc_abort(I)::check();
	  return mln_concrete(I)();
	}

	template<typename I>
	inline
	mln_concrete(I)
	enlarge_dispatch(const Image<I>& input, unsigned n)
	{
	  return enlarge_dispatch(exact(input), mln_value(I)(), n);
	}

      } // end of namespace mln::world::binary_2d::internal



      // Facade

      template <typename I>
      inline
      mln_concrete(I)
      enlarge(const Image<I>& input, unsigned n)
      {
	trace::entering("mln::world::binary_2d::enlarge");

	mln_precondition(exact(input).is_valid());
	typedef mln_site(I) S;
	mlc_bool(S::dim == 2)::check();

	mln_concrete(I) output;
	if (n == 0)
	    output = duplicate(input);
	else
	    output = internal::enlarge_dispatch(input, n);

	trace::exiting("mln::world::binary_2d::enlarge");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::binary_2d

  } // end of namespace mln::world

} // mln

#endif // ! MLN_WORLD_BINARY_2D_ENLARGE_HH
