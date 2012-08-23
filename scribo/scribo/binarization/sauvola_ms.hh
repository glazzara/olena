// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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


#ifndef SCRIBO_BINARIZATION_SAUVOLA_MS_HH
# define SCRIBO_BINARIZATION_SAUVOLA_MS_HH

/// \file
///
/// \brief Binarize an image using a multi-scale implementation of
/// Sauvola's algoritm.
///
/// \fixme Use the integral image for successive subsampling.


# include <mln/core/alias/neighb2d.hh>
# include <mln/data/fill.hh>
# include <mln/data/compare.hh>

# include <mln/subsampling/antialiased.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <mln/data/split.hh>

# include <mln/value/int_u8.hh>
# include <mln/border/mirror.hh>
# include <mln/border/adjust.hh>
# include <mln/border/resize.hh>

# include <mln/core/box_runend_piter.hh>

# include <mln/util/couple.hh>

# include <mln/extension/adjust.hh>

# include <scribo/subsampling/integral_single_image.hh>

# include <scribo/core/macros.hh>

# include <scribo/binarization/internal/sauvola_ms_functor.hh>

# include <scribo/canvas/integral_browsing.hh>

# include <scribo/util/init_integral_image.hh>
# include <scribo/util/integral_sub_sum_sum2_functor.hh>
# include <scribo/util/compute_sub_domains.hh>

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
#  include <scribo/binarization/internal/local_threshold_debug.hh>
#  include <mln/io/pgm/save.hh>
#  include <mln/debug/filename.hh>
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

# include <mln/util/timer.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;
    using value::int_u8;


    /*! \brief Binarize an image using a multi-scale implementation of
        Sauvola's algoritm.

      \param[in] input_1 A grayscale or a color image.
      \param[in] w_1 The window size used to compute stats.
      \param[in] s The scale factor used for the first subscaling.
      \param[in] lambda_min_1 Size of the objects kept at scale 1.
      \param[in] K Sauvola's formulae parameter.


      \p w_1 and \p lambda_min_1 are expressed according to the image
      at scale 0, i.e. the original size.

      \return A Boolean image.
     */
    template <typename I>
    mln_ch_value(I,bool)
    sauvola_ms(const Image<I>& input_1_, unsigned w_1, unsigned s, double K);

    /// \overload
    /// K is set to 0.34.
    //
    template <typename I>
    mln_ch_value(I,bool)
    sauvola_ms(const Image<I>& input_1, unsigned w_1, unsigned s);



# ifndef MLN_INCLUDE_ONLY


    // Routines

    namespace internal
    {

      using namespace mln;


      template <typename V>
      V my_find_root(image2d<V>& parent, const V& x)
      {
	if (parent.element(x) == x)
	  return x;
	return parent.element(x) = my_find_root(parent,
						parent.element(x));
      }


      inline
      image2d<int_u8>
      compute_t_n_and_e_2(const image2d<int_u8>& sub, image2d<int_u8>& e_2,
			  unsigned lambda_min, unsigned lambda_max,
			  unsigned s,
			  unsigned q, unsigned i, unsigned w,
			  const image2d<mln::util::couple<double,double> >& integral_sum_sum_2,
			  double K)
      {
	typedef image2d<int_u8> I;
	typedef point2d P;

	// Cast to float is needed on MacOS X.
	unsigned ratio = unsigned(std::pow(float(q), float(i - 2u)));  // Ratio in comparison to e_2

	unsigned
	  w_local = w * ratio,
	  w_local_h = w_local,
	  w_local_w = w_local;

	// Make sure the window fits in the image domain.
	if (w_local_w >= static_cast<const unsigned>(integral_sum_sum_2.ncols()))
	{
	  w_local_w = std::min(integral_sum_sum_2.ncols(),
			       integral_sum_sum_2.nrows()) - integral_sum_sum_2.border();
	  w_local_h = w_local_w;
	  trace::warning("integral_browsing - Adjusting window width since it"
			 " was larger than image width.");
	}
	if (w_local_h >= static_cast<const unsigned>(integral_sum_sum_2.nrows()))
	{
	  w_local_h = std::min(integral_sum_sum_2.nrows(),
			       integral_sum_sum_2.ncols()) - integral_sum_sum_2.border();
	  w_local_w = w_local_h;
	  trace::warning("integral_browsing - Adjusting window height since it"
			 " was larger than image height.");
	}

	if (! (w_local % 2))
	{
	  --w_local_w;
	  ++w_local_h;
	}


	// 1st pass
	scribo::binarization::internal::sauvola_ms_functor< image2d<int_u8> >
	  f(sub, K, SCRIBO_DEFAULT_SAUVOLA_R);
	scribo::canvas::integral_browsing(integral_sum_sum_2,
					  ratio,
					  w_local_w, w_local_h,
					  s,
					  f);

	// 2nd pass
	{
	  mln::util::array<mln_value_(I) *> ptr(ratio);
	  unsigned nrows = geom::nrows(e_2);

	  mln_box_runend_piter_(I) sp(sub.domain()); // Backward.
	  unsigned ncols = sp.run_length();
	  for_all(sp)
	  {
	    unsigned p = &sub(sp) - sub.buffer(); // Offset
	    P site = sp;

	    {
	      P tmp = site * ratio;

	      // FIXME: to be removed!
	      if (tmp.row() + ratio >= nrows)
		ptr.resize(nrows - tmp.row());

	      ptr(0) = &e_2(tmp);
	      // FIXME: pointers could just be updated with an offset.
	      for (unsigned j = 1; j < ptr.size(); ++j)
	      {
		tmp[0] += 1;
		ptr(j) = & e_2(tmp);
	      }
	    }

	    for (unsigned j = 0; j < ncols; ++j)
	    {
	      if (f.msk.element(p))
	      {

		mln_site_(I) sq = site * ratio;

		if (f.parent.element(p) == p)
		{
		  // test over the component cardinality
		  f.msk.element(p) = f.card.element(p) > lambda_min;
//		    && f.card.element(p) < lambda_max;
#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
		  f.full_msk.element(p) = true;
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

		  if (f.msk.element(p) && e_2(sq) == 0u)
		  {
		    for (unsigned l = 0; l < ptr.size(); ++l)
		      std::memset(ptr(l), i, ratio * sizeof(mln_value_(I)));
		  }

		}
		else
		{
		  // Propagation
		  f.msk.element(p) = f.msk.element(f.parent.element(p));
#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
		  f.full_msk.element(p) = f.full_msk.element(f.parent.element(p));
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

		  if (f.msk.element(p) && e_2(sq) == 0u)
		  {
		    for (unsigned l = 0; l < ptr.size(); ++l)
		      std::memset(ptr(l), i, ratio * sizeof(mln_value_(I)));
		  }

		}
	      }

	      for (unsigned l = 0; l < ptr.size(); ++l)
		ptr(l) -= ratio;

	      --site[1];
	      --p;
	    }

	  }
	} // end of 2nd pass


#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	if (internal::threshold_image_output)
	  io::pbm::save(f.msk,
			mln::debug::filename(internal::threshold_image_output));
	if (internal::full_threshold_image_output)
	  io::pbm::save(f.full_msk,
			mln::debug::filename(internal::full_threshold_image_output));
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	return f.t_sub;
      }



      template <typename I, typename J, typename K>
      mln_ch_value(I, bool)
      multi_scale_binarization(const I& in, const J& e2,
			       const mln::util::array<K>& t_ima,
			       unsigned s)
      {
	mln_ch_value(I,bool) out;
	initialize(out, in);

	typedef const mln_value(K)* ptr_type;

	// Warning: if there are pixels with value different from 2, 3
	// or 4 in e2, it will crash because of that array...
	ptr_type ptr_t[5];
	ptr_t[2] = & t_ima[2].at_(0, 0);
	ptr_t[3] = & t_ima[3].at_(0, 0);
	ptr_t[4] = & t_ima[4].at_(0, 0);


	const mln_value(J)* ptr_e2   = & e2.at_(0, 0);
	const mln_value(I)* ptr__in = & in.at_(0, 0);
	bool*    ptr__out = & out.at_(0, 0);


	// Since we iterate from a smaller image in the largest ones and
	// image at scale 1 does not always have a size which can be
	// divided by (4*s), some sites in the border may not be processed
	// and we must skip them.
	int more_offset = - ((4 * s) - in.ncols() % (4 * s));

	if (more_offset == - (static_cast<int>(4*s)))
	  more_offset = 0; // No offset needed.

	const int
	  nrows4 = t_ima[4].nrows(),
	  ncols4 = t_ima[4].ncols(),

	  delta1  = in.delta_index(dpoint2d(+1, -(s - 1))),
	  delta1b = in.delta_index(dpoint2d(+1, -(s + s - 1))),
	  delta1c = in.delta_index(dpoint2d(-(s + s - 1), +1)),
	  delta1d = in.delta_index(dpoint2d(+1, -(s * 4 - 1))),
	  delta1e = in.delta_index(dpoint2d(-(s * 4 - 1), +1)),
	  delta1f = in.delta_index(dpoint2d(-(s - 1), +1)),

	  delta2  = t_ima[2].delta_index(dpoint2d(+1, -1)),
	  delta2b = t_ima[2].delta_index(dpoint2d(+1, -3)),
	  delta2c = t_ima[2].delta_index(dpoint2d(-3, +1)),

	  delta3  = t_ima[3].delta_index(dpoint2d(+1, -1)),

	  eor1 = in.delta_index(dpoint2d(+4 * s, - in.ncols())) + more_offset,
	  eor2 = t_ima[2].delta_index(dpoint2d(+4,- t_ima[2].ncols())),
	  eor3 = t_ima[3].delta_index(dpoint2d(+2,- t_ima[3].ncols())),
	  eor4 = t_ima[4].delta_index(dpoint2d(+1,- t_ima[4].ncols()));

	mln_value(J) threshold;
	for (int row4 = 0; row4 < nrows4; ++row4)
	{
	  for (int col4 = 0; col4 < ncols4; ++col4)
	  {
	    // top left  1
	    {
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1b; ptr__in += delta1b;
	      }

	      ptr_t[2] += delta2; ptr_e2 += delta2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;

	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1c; ptr__in += delta1c;
	      }

	      ptr_t[2] -= delta2; ptr_e2 -= delta2;
	    }

	    // top right 1
	    ptr_t[3] += 1;
	    {
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1b; ptr__in += delta1b;
	      }

	      ptr_t[2] += delta2; ptr_e2 += delta2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1d; ptr__in += delta1d;
	      }

	      ptr_t[2] += delta2b; ptr_e2 += delta2b;
	    }

	    // bot left  1
	    ptr_t[3] += delta3;
	    {
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1b; ptr__in += delta1b;
	      }

	      ptr_t[2] += delta2; ptr_e2 += delta2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1c; ptr__in += delta1c;
	      }

	      ptr_t[2] -= delta2; ptr_e2 -= delta2;
	    }

	    // bot right 1
	    ptr_t[3] += 1;
	    {
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1b; ptr__in += delta1b;
	      }

	      ptr_t[2] += delta2; ptr_e2 += delta2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1f; ptr__in += delta1f;
	      }

	      ++ptr_t[2]; ++ptr_e2;
	      threshold = *ptr_t[*ptr_e2];
	      {
		for (unsigned i = 1; i < s; ++i)
		{
		  for (unsigned j = 1; j < s; ++j)
		  {
		    *ptr__out = *ptr__in <= threshold;
		    ++ptr__out; ++ptr__in;
		  }

		  *ptr__out = *ptr__in <= threshold;
		  ptr__out += delta1; ptr__in += delta1;
		}

		for (unsigned j = 1; j < s; ++j)
		{
		  *ptr__out = *ptr__in <= threshold;
		  ++ptr__out; ++ptr__in;
		}
		*ptr__out = *ptr__in <= threshold;
		ptr__out += delta1e; ptr__in += delta1e;
	      }
	    }

	    // bot right -> next top left
	    ptr_t[2] += delta2c; ptr_e2 += delta2c;
	    ptr_t[3] = ptr_t[3] - delta3;
	    ptr_t[4] += 1;
	  }

	  // eof -> next bof
	  ptr__out += eor1; ptr__in  += eor1;
	  ptr_t[2] += eor2; ptr_e2 += eor2;
	  ptr_t[3] += eor3;
	  ptr_t[4] += eor4;
	}

	return out;
      }

    } // end of namespace scribo::binarization::internal



    // Implementation

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	mln_ch_value(I,bool)
	sauvola_ms(const Image<I>& input_1_, unsigned w_1,
		   unsigned s, double K)
	{
	  trace::entering("scribo::binarization::sauvola_ms");

	  const I& input_1 = exact(input_1_);

	  mlc_is_a(mln_value(I), value::Scalar)::check();
	  mln_precondition(input_1.is_valid());

	  dpoint2d none(0, 0);

	  unsigned lambda_min_1 = w_1 / 2;

	  // Number of subscales.
	  unsigned nb_subscale = 3;

	  // Window size.
	  unsigned w_work = w_1 / s;        // Scale 2


	  // Subscale step.
	  unsigned q = 2;

	  unsigned lambda_min_2 = lambda_min_1 / s;
	  unsigned lambda_max_2 = lambda_min_2 * q;


	  mln::util::array<I> t_ima;

	  // Make sure t_ima indexes start from 2.
	  {
	    I dummy(1,1);
	    for (unsigned i = 0; i < nb_subscale + 2; ++i)
	      t_ima.append(dummy);
	  }

	  mln::util::array<I> sub_ima;

	  // Make sure sub_ima indexes start from 2.
	  {
	    I dummy(1,1);
	    sub_ima.append(dummy);
	    sub_ima.append(dummy);
	  }

	  mln::util::array<mln::util::couple<box2d, unsigned> >
	    sub_domains = scribo::util::compute_sub_domains(input_1,
							    nb_subscale, s);

	  border::adjust(input_1, sub_domains(1).second());
	  border::mirror(input_1);


	  // Resize input and compute integral images.
	  typedef image2d<mln::util::couple<double,double> > integral_t;
	  integral_t integral_sum_sum_2;

	  mln::util::timer t;
	  t.start();

	  // Subsampling from scale 1 to 2.
	  {
	    scribo::util::integral_sub_sum_sum2_functor<I, double>
	      fi(s, sub_domains[2].first(), sub_domains[2].second());

	    integral_sum_sum_2 = scribo::util::init_integral_image(input_1, s, fi,
								   sub_domains[2].first(),
								   sub_domains[2].second());
	    sub_ima.append(fi.sub);
	  }

	  t.stop();
	  std::cout << "1. subsampling and integral - " << t << std::endl;
	  t.restart();

	  // Subsampling to scale 3 and 4.
	  //
	  // FIXME: we may use the integral image to compute
	  // subsampled images -> faster and more precise.
	  for (unsigned i = 3; i <= nb_subscale + 1; ++i)
	    sub_ima.append(mln::subsampling::antialiased(sub_ima[i - 1], q,
							 sub_domains[i].first(),
							 sub_domains[i].second()));

	  t.stop();
	  std::cout << "2. More subsampling - " << t << std::endl;
	  t.restart();

	  // Compute threshold images.
	  image2d<int_u8> e_2;
	  initialize(e_2, sub_ima[2]);
	  data::fill(e_2, 0u);

	  // Highest scale -> no maximum component size.
	  {
	    int i = sub_ima.size() - 1;
	    // Cast to float is needed on MacOS X.
	    unsigned ratio = unsigned(std::pow(float(q), float(i - 2))); // Ratio compared to e_2
	    t_ima[i] = internal::compute_t_n_and_e_2(sub_ima[i], e_2,
						     lambda_min_2 / ratio,
						     mln_max(unsigned),
						     s,
						     q, i, w_work,
						     integral_sum_sum_2,
						     K);
	  }

	  // Other scales -> maximum and minimum component size.
	  {
	    for (int i = sub_ima.size() - 2; i > 2; --i)
	    {
	      // Cast to float is needed on MacOS X.
	      unsigned ratio = unsigned(std::pow(float(q), float(i - 2))); // Ratio compared to e_2
	      t_ima[i] = internal::compute_t_n_and_e_2(sub_ima[i], e_2,
						       lambda_min_2 / ratio,
						       lambda_max_2 / ratio,
						       s,
						       q, i, w_work,
						       integral_sum_sum_2,
						       K);
	    }
	  }

	  // Lowest scale -> no minimum component size.
	  {
	    t_ima[2] = internal::compute_t_n_and_e_2(sub_ima[2], e_2, 0,
						     lambda_max_2,
						     s, 1, 2, w_work,
						     integral_sum_sum_2,
						     K);
	  }

	  t.stop();
	  std::cout << "3. Multi-scale processing - " << t << std::endl;
	  t.restart();


#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	  if (internal::scale_image_output)
	    io::pgm::save(e_2,
			  mln::debug::filename(internal::scale_image_output));
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	  // Propagate scale values.
	  e_2 = transform::influence_zone_geodesic(e_2, c8());

	  t.stop();
	  std::cout << "4. Influence Zone on Scale image - " << t << std::endl;
	  t.restart();


#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
 	  if (internal::scale_iz_image_output)
 	    io::pgm::save(e_2,
			  mln::debug::filename(internal::scale_iz_image_output));
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	  // Binarize
	  image2d<bool>
	    output = internal::multi_scale_binarization(input_1, e_2, t_ima, s);

	  t.stop();
	  std::cout << "5. Final binarization - " << t << std::endl;
	  t.restart();

	  trace::exiting("scribo::binarization::sauvola_ms");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic


    } // end of namespace scribo::binarization::impl


    // Facade

    template <typename I>
    mln_ch_value(I,bool)
    sauvola_ms(const Image<I>& input_1_, unsigned w_1,
	       unsigned s, double K)
    {
      trace::entering("scribo::binarization::sauvola_ms");

      mln_precondition(exact(input_1_).is_valid());
      // Gray level images ONLY.
      mlc_is_not_a(mln_value(I), value::Vectorial)::check();
      mlc_is_not(mln_value(I), bool)::check();

      mln_ch_value(I,bool)
	output = impl::generic::sauvola_ms(exact(input_1_), w_1, s, K);

      trace::exiting("scribo::binarization::sauvola_ms");
      return output;
    }


    template <typename I>
    mln_ch_value(I,bool)
    sauvola_ms(const Image<I>& input_1, unsigned w_1, unsigned s)
    {
      return sauvola_ms(input_1, w_1, s, SCRIBO_DEFAULT_SAUVOLA_K);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // SCRIBO_BINARIZATION_SAUVOLA_MS_HH
