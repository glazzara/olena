// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DATA_SORT_OFFSETS_HH
# define MLN_DATA_SORT_OFFSETS_HH

/// \file
/// \brief Sort_Offsets the contents of an image into another one.
///
/// \todo Factor code + optimize.

# include <algorithm>

# include <mln/core/concept/image.hh>
# include <mln/histo/compute.hh>
# include <mln/util/array.hh>
# include <mln/util/ord.hh>
# include <mln/geom/nsites.hh>


namespace mln
{

  namespace data
  {

    /// Sort pixel offsets of the image \p input wrt increasing pixel
    /// values.
    ///
    template <typename I>
    util::array<unsigned>
    sort_offsets_increasing(const Image<I>& input);


    //     /// Sort pixel offsets of the image \p input wrt decreasing pixel
    //     /// values.
    //     ///
    //     template <typename I>
    //     util::array<unsigned>
    //     sort_offsets_decreasing(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	// increasing

	template <typename I>
	struct value_offset_less_
	{
	  const I& ima_;
	  inline value_offset_less_(const I& ima) : ima_(ima) {}
	  inline bool operator()(unsigned lhs, unsigned rhs) const
	  {
	    return util::ord_strict(ima_.element(lhs), ima_.element(rhs))
	      || (ima_.element(lhs) == ima_.element(rhs)
		  &&
		  lhs < rhs);
	  }
	};

	template <typename I>
	inline
	util::array<unsigned>
	sort_offsets_increasing(const Image<I>& input_)
	{
	  mln_trace("data::impl::generic::sort_offsets_increasing");

	  const I& input = exact(input_);

	  util::array<unsigned> v;
	  v.reserve(input.nelements());
	  mln_fwd_pixter(const I) pxl(input);
	  for_all(pxl)
	    v.append(pxl.offset());
	  std::sort(v.hook_std_vector_().begin(), v.hook_std_vector_().end(),
		    value_offset_less_<I>(input));

	  return v;
	}


	// decreasing

	template <typename I>
	struct value_offset_greater_
	{
	  const I& ima_;
	  inline value_offset_greater_(const I& ima) : ima_(ima) {}
	  inline bool operator()(unsigned lhs, unsigned rhs) const
	  {
	    return util::ord_strict(ima_.element(rhs), ima_.element(lhs))
	      || (ima_.element(lhs) == ima_.element(rhs)
		  &&
		  lhs > rhs);
	  }
	};

	template <typename I>
	inline
	util::array<unsigned>
	sort_offsets_decreasing(const Image<I>& input_)
	{
	  mln_trace("data::impl::generic::sort_offsets_decreasing");

	  const I& input = exact(input_);

	  util::array<unsigned> v;
	  v.reserve(input.nelements());
	  mln_fwd_pixter(const I) pxl(input);
	  for_all(pxl)
	    v.append(pxl.offset());
	  std::sort(v.hook_std_vector_().begin(), v.hook_std_vector_().end(),
		    value_offset_greater_<I>(input));

	  return v;
	}


      } // end of namespace mln::data::impl::generic



      // increasing

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_increasing_radix(const Image<I>& input_)
      {
	mln_trace("data::impl::sort_offsets_increasing_radix");

	const I& input = exact(input_);

	typedef mln_vset(I) S;
	const S& vset = input.values_eligible();
	const unsigned n = vset.nvalues();

	// h
	histo::array<mln_value(I)> h = histo::compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
	loc[0] = 0;
	for (unsigned i = 1; i != n; ++i)
	  loc[i] = loc[i-1] + h[i-1];

	// computing output data
	util::array<unsigned> vec(geom::nsites(input));
	mln_fwd_pixter(const I) pxl(input);
	for_all(pxl)
	  vec[loc[vset.index_of(pxl.val())]++] = pxl.offset();

	return vec;
      }


      // decreasing

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_decreasing_radix(const Image<I>& input_)
      {
	mln_trace("data::impl::sort_offsets_decreasing_radix");

	const I& input = exact(input_);

	typedef mln_vset(I) S;
	const S& vset = input.values_eligible();
	const unsigned n = vset.nvalues();

	// h
	histo::array<mln_value(I)> h = histo::compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
      	loc[n-1] = 0;
      	for (int i = n - 2; i >= 0; --i)
      	  loc[i] = loc[i+1] + h[i+1];

	// computing output data
	util::array<unsigned> vec(geom::nsites(input));
	mln_fwd_pixter(const I) pxl(input);
	for_all(pxl)
	  vec[loc[vset.index_of(pxl.val())]++] = pxl.offset();

	return vec;
      }


    } // end of namespace mln::data::impl



    namespace internal
    {

      // increasing

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_increasing_dispatch(trait::image::quant::any,
				       const Image<I>& input)
      {
	return impl::generic::sort_offsets_increasing(input);
      }

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_increasing_dispatch(trait::image::quant::low,
				       const Image<I>& input)
      {
	return impl::sort_offsets_increasing_radix(input);
      }

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_increasing_dispatch(const Image<I>& input)
      {
	return sort_offsets_increasing_dispatch(mln_trait_image_quant(I)(),
						input);
      }

      // decreasing

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_decreasing_dispatch(trait::image::quant::any,
				       const Image<I>& input)
      {
	return impl::generic::sort_offsets_decreasing(input);
      }

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_decreasing_dispatch(trait::image::quant::low,
				       const Image<I>& input)
      {
	return impl::sort_offsets_decreasing_radix(input);
      }

      template <typename I>
      inline
      util::array<unsigned>
      sort_offsets_decreasing_dispatch(const Image<I>& input)
      {
	return sort_offsets_decreasing_dispatch(mln_trait_image_quant(I)(),
						input);
      }

    } // end of namespace mln::data::internal



    // Facades.

    template <typename I>
    inline
    util::array<unsigned>
    sort_offsets_increasing(const Image<I>& input)
    {
      mln_trace("data::sort_offsets_increasing");
      mlc_is(mln_trait_image_speed(I),
	     trait::image::speed::fastest)::check();

      mln_precondition(exact(input).is_valid());
      util::array<unsigned> output = internal::sort_offsets_increasing_dispatch(input);

      return output;
    }

    template <typename I>
    inline
    util::array<unsigned>
    sort_offsets_decreasing(const Image<I>& input)
    {
      mln_trace("data::sort_offsets_decreasing");
      mlc_is(mln_trait_image_speed(I),
	     trait::image::speed::fastest)::check();

      mln_precondition(exact(input).is_valid());
      util::array<unsigned> output = internal::sort_offsets_decreasing_dispatch(input);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_SORT_OFFSETS_HH
