// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_SUBSAMPLING_INTEGRAL_HH
# define MLN_SUBSAMPLING_INTEGRAL_HH

/// \file
///
/// Both subsampling and integral image computation.

#include <mln/core/concept/image.hh>



namespace mln
{

  namespace subsampling
  {

    /// FIXME: Doc.

    template <typename I>
    mln_concrete(I)
    integral(const Image<I>& input, unsigned scale);



# ifndef MLN_INCLUDE_ONLY


    // Implementation.

    namespace impl
    {


      template <unsigned scale, typename I>
      inline
      mln_concrete(I)
      integral_(const Image<I>& input_)
      {
	trace::entering("subsampling::impl::integral_");

	const I& input = exact(input_);
	const unsigned scale2 = scale * scale, round_it = scale2 / 2;

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	typedef mln_site(I) P;

	box<P> b = make::box2d(input.nrows() / scale, input.ncols() / scale);
	const unsigned nrows = b.nrows(), ncols = b.ncols();

	typedef mln_concrete(I) O;
	O output(b, 0); // no external border in 'output'
	V* po = output.buffer();

	unsigned
	  tile_cr = input.delta_index(dpoint2d(scale, - ncols * scale)),
	  cr      = input.delta_index(dpoint2d(1, - (int)scale));

	const V* pi = & input.at_(0, 0); // first point of tiles
	for (unsigned row = 0; row < nrows; ++row)
	  {
	    S hsum = 0, hsum2 = 0; // horizontal integral
	    for (unsigned col = 0; col < ncols; ++col)
	      {
		S sum = 0, sum2 = 0;
		const V* ptr = pi;

		for (unsigned r = 0; r < scale; ++r)
		  {
		    for (unsigned c = 0; c < scale; ++c)
		      {
			sum  += *ptr;
			// sum2 += *ptr * *ptr;
			++ptr;
		      }
		    ptr += cr;
		  }

		// ...
		pi += scale;
		*po++ = (round_it + sum) / scale2;
	      }
	    pi += tile_cr;
	  }

	trace::exiting("subsampling::impl::integral_");
	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      integral(const Image<I>& input, unsigned scale)
      {
	//       mln_precondition(input.nrows() % scale == 0);
	//       mln_precondition(input.ncols() % scale == 0);
	if (scale == 3)
	  return integral_<3>(input);
	else
	  std::cerr << "NYI!" << std::endl;
      }

    } // end of namespace mln::subsampling::impl




    // Facades.

    template <typename I>
    inline
    mln_concrete(I)
    integral(const Image<I>& input_, unsigned scale)
    {
      trace::entering("subsampling::integral");

	const I& input = exact(input_);
      
      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);
      mln_precondition(scale > 1);

      mln_concrete(I) output;
      output = impl::integral(input, scale);

      trace::exiting("subsampling::integral");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_INTEGRAL_HH
