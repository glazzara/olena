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

#ifndef SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_MS_FUNCTOR_HH
# define SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_MS_FUNCTOR_HH

/// \file
///
///

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/extension/fill.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/fill.hh>

# include <scribo/binarization/internal/sauvola_formula.hh>

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
#  include <scribo/binarization/internal/local_threshold_debug.hh>
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

#include <mln/value/int_u8.hh>

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;


      template <typename I>
      struct sauvola_ms_functor
      {
	const I& input;

	const image2d<value::int_u8>& e_2;
	unsigned i;
	unsigned q;

	mln_fwd_pixter(const I) pxl;
	double res;
	image2d<unsigned> parent;
	image2d<unsigned> card;
	image2d<bool> msk;
	image2d<value::int_u8> t_sub;

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	image2d<bool> full_msk;
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	unsigned n_nbhs;
	mln::util::array<int> dp;

	double K_;
	double R_;

	sauvola_formula formula_;

	sauvola_ms_functor(const I& input, double K, double R, const image2d<value::int_u8>&e_2, unsigned i, unsigned q);

	void exec(double mean, double stddev);
	void end_of_row(int row);
	void finalize();

      };


# ifndef MLN_INCLUDE_ONLY

      inline
      unsigned my_find_root(image2d<unsigned>& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	return parent.element(x) = my_find_root(parent,
						parent.element(x));
      }


      template <typename I>
      sauvola_ms_functor<I>::sauvola_ms_functor(const I& input, double K, double R, const image2d<value::int_u8>&e_2, unsigned i, unsigned q)
	: input(input),
	  e_2(e_2),
	  i(i),
	  q(q),
	  pxl(input),
	  K_(K),
	  R_(R)
      {
	res = 0;
	pxl.start();

	initialize(t_sub, input);
	initialize(parent, input);
	initialize(msk, input);

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	initialize(full_msk, input);
	mln::extension::fill(full_msk, false);
	initialize(debug_mean, input);
	initialize(debug_stddev, input);
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	mln::extension::fill(msk, false);

	initialize(card, input);
	data::fill(card, 1);

	dp = negative_offsets_wrt(input, c8());
	n_nbhs = dp.nelements();
      }


      template <typename I>
      void
      sauvola_ms_functor<I>::exec(double mean, double stddev)
      {
	mln_precondition(pxl.is_valid());

	unsigned p = pxl.offset();

	value::int_u8 t_p;
	mln::convert::from_to(formula_(mean, stddev, K_, R_), t_p);

	// point2d pi = input.point_at_index(p);
	// pi.row() *= std::pow(q, i - 2);
	// pi.col() *= std::pow(q, i - 2);

	// point2d pi_up = pi;
	// pi_up.row() -= std::pow(q, i - 2);

	// point2d pi_up_left = pi;
	// pi_up_left.row() -= std::pow(q, i - 2);
	// pi_up_left.col() -= std::pow(q, i - 2);

	// point2d pi_up_right = pi;
	// pi_up_right.row() -= std::pow(q, i - 2);
	// pi_up_right.col() += std::pow(q, i - 2);

	// point2d pi_down = pi;
	// pi_down.row() += std::pow(q, i - 2);

	// point2d pi_down_left = pi;
	// pi_down_left.row() += std::pow(q, i - 2);
	// pi_down_left.col() -= std::pow(q, i - 2);

	// point2d pi_down_right = pi;
	// pi_down_right.row() += std::pow(q, i - 2);
	// pi_down_right.col() += std::pow(q, i - 2);

	// point2d pi_left = pi;
	// pi_left.col() -= std::pow(q, i - 2);

	// point2d pi_right = pi;
	// pi_right.col() += std::pow(q, i - 2);





// 	if (e_2(pi) != 0)   // Already retrieved from another scale.
// //	    || e_2(pi_up) != 0 || e_2(pi_down) != 0 || e_2(pi_left) != 0 || e_2(pi_right) != 0
// //	    || e_2(pi_up_left) != 0 || e_2(pi_up_right) != 0 || e_2(pi_down_left) != 0 || e_2(pi_down_right) != 0)
// 	  msk.element(p) = false;
// 	else
	msk.element(p) = input.element(p) < t_p;

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	full_msk.element(p) = msk.element(p);
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	t_sub.element(p) = t_p;
	if (! msk.element(p))
	{
	  pxl.next();
	  return;
	}
	parent.element(p) = p;
	for (unsigned i = 0; i < n_nbhs; ++i)
	{
	  unsigned n = p + dp[i];
	  if (! msk.element(n))
	    continue;
	  unsigned r = my_find_root(parent, n);
	  if (r != p)
	  {
	    parent.element(r) = p;
	    card.element(p) += card.element(r);
	  }
	}

	pxl.next(); // next pixel
      }


      template <typename I>
      void
      sauvola_ms_functor<I>::end_of_row(int row)
      {
	(void) row;
      }

      template <typename I>
      void sauvola_ms_functor<I>::finalize()
      {
	mln_assertion(! pxl.is_valid());
      }

#endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_MS_FUNCTOR_HH
