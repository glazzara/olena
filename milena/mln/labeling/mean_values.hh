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

#ifndef MLN_LABELING_MEAN_VALUES_HH
# define MLN_LABELING_MEAN_VALUES_HH

/// \file
///
/// Construct from a labeled image a new image, the labels values are
/// replaced by the components mean values.
///
/// \todo handle mean value for label 0 correctly.
/// \todo merge rgb and generic version.


# include <mln/core/concept/image.hh>
# include <mln/core/alias/vec3d.hh>

# include <mln/accu/stat/mean.hh>

# include <mln/data/transform.hh>

# include <mln/labeling/compute.hh>

# include <mln/literal/colors.hh>


namespace mln
{

  // Forward declaration
  namespace value
  {
    template <unsigned n> struct rgb;
  }


  namespace labeling
  {

    template <typename I, typename L>
    mln_concrete(I)
    mean_colors(const Image<I>& input,
		const Image<L>& lbl, mln_value(L) nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename L>
      void
      mean_values_tests(const Image<I>& input,
			const Image<L>& lbl, mln_value(L) nlabels)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(lbl).is_valid());
	(void) input;
	(void) lbl;
	(void) nlabels;
      }

    } // end of namespace mln::labeling::internal


    namespace impl
    {

      namespace generic
      {

        template <typename I, typename L>
        mln_concrete(I)
        mean_values(const Image<I>& input_,
		    const Image<L>& lbl_, mln_value(L) nlabels)
	{
	  mln_trace("mln::labeling::impl::generic::mean_values");

	  internal::mean_values_tests(input_, lbl_, nlabels);

	  const I& input = exact(input_);
	  const L& lbl = exact(lbl_);
	  typedef mln_value(L) LV;
	  typedef mln_value(I) IV;

	  util::array<float> m_3f
	      = labeling::compute(accu::stat::mean<IV>(),
				  input, // input color image
				  lbl, // watershed labeling
				  nlabels);
	  m_3f[0] = 0.f;

	  util::array<IV> m;
	  convert::from_to(m_3f, m);
	  m[0] = 150u; //FIXME: handle label 0 correctly.

	  mln_concrete(I) output = data::transform(lbl, m);

	  return output;
	}

      }

      template <typename I, typename L>
      mln_concrete(I)
      mean_values_rgb(const Image<I>& input_,
		      const Image<L>& lbl_, mln_value(L) nlabels)
      {
	mln_trace("mln::labeling::impl::mean_values_rgb");

	internal::mean_values_tests(input_, lbl_, nlabels);

	const I& input = exact(input_);
	const L& lbl = exact(lbl_);

	util::array<vec3d_f> m_3f = labeling::compute(accu::stat::mean<mln_value(I)>(),
	    input, // input color image
	    lbl, // watershed labeling
	    nlabels);
	m_3f[0] = literal::zero;

	util::array<mln_value(I)> m;
	convert::from_to(m_3f, m);
	m[0] = literal::yellow; //FIXME: handle label 0 correctly.

	mln_concrete(I) output = data::transform(lbl,
	    convert::to< fun::i2v::array<mln_value(I)> >(m));


	return output;
      }

    } // end of namespace mln::morpho::impl


    namespace internal
    {

      template <unsigned n, typename I, typename L>
      mln_concrete(I)
      mean_values_dispatch(const value::rgb<n>&,
			   const Image<I>& input,
			   const Image<L>& lbl, mln_value(L) nlabels)
      {
	return impl::mean_values_rgb(input, lbl, nlabels);
      }

      template <typename I, typename L>
      mln_concrete(I)
      mean_values_dispatch(const mln_value(I)&,
			   const Image<I>& input,
			   const Image<L>& lbl, mln_value(L) nlabels)
      {
	return impl::generic::mean_values(input, lbl, nlabels);
      }

      template <typename I, typename L>
      mln_concrete(I)
      mean_values_dispatch(const Image<I>& input,
			   const Image<L>& lbl, mln_value(L) nlabels)
      {
	return mean_values_dispatch(mln_value(I)(), input, lbl, nlabels);
      }

    } // end of namespace mln::morpho::internal



    // Facade

    template <typename I, typename L>
    mln_concrete(I)
    mean_values(const Image<I>& input,
		const Image<L>& lbl, mln_value(L) nlabels)

    {
      mln_trace("mln::labeling::mean_values");

      internal::mean_values_tests(input, lbl, nlabels);

      mln_concrete(I) output = internal::mean_values_dispatch(input, lbl, nlabels);

      return output;
    }


# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln

#endif // ! MLN_LABELING_MEAN_VALUES_HH
