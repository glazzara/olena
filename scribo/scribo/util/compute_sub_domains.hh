// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_UTIL_COMPUTE_SUB_DOMAINS_HH
# define SCRIBO_UTIL_COMPUTE_SUB_DOMAINS_HH

/// \file
///
///

# include <mln/core/image/image2d.hh>

namespace scribo
{

  namespace util
  {

    using namespace mln;

    template <typename I>
    mln::util::array<mln::util::couple<mln_domain(I), unsigned> >
    compute_sub_domains(const Image<I>& ima, unsigned n_scales, unsigned s);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      inline
      unsigned sub(unsigned nbr, unsigned down_scaling)
      {
	return (nbr + down_scaling - 1) / down_scaling;
      }

    } // end of namespace scribo::util::internal



    template <typename I>
    mln::util::array<mln::util::couple<mln_domain(I), unsigned> >
    compute_sub_domains(const Image<I>& ima_, unsigned n_scales, unsigned s)
    {
      trace::entering("scribo::util::compute_sub_domains");

      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      mln::util::array<mln::util::couple<unsigned, unsigned> > n(n_scales + 2);

      n(1) = mln::make::couple(ima.nrows(), ima.ncols());
      n(2) = mln::make::couple(internal::sub(n(1).first(), s),
			       internal::sub(n(1).second(), s));
      for (unsigned i = 3; i <= n_scales + 1; ++i)
	n(i) = mln::make::couple(internal::sub(n(i - 1).first(), 2),
				 internal::sub(n(i - 1).second(), 2));


      mln::util::array<mln::util::couple<mln_domain(I), unsigned> > out(n.size());
      out(0) = mln::make::couple(mln::make::box2d(1,1), 1u);
      out(1) = mln::make::couple(mln::make::box2d(ima.nrows(),
						  ima.ncols()), 2u);
      out(n_scales + 1) = mln::make::couple(
	mln::make::box2d(n(n_scales + 1).first(),
			 n(n_scales + 1).second()), 1u);

      for (unsigned i = n_scales; i > 1; --i)
	out(i) = mln::make::couple(
	  mln::make::box2d(2 * out(i + 1).first().nrows(),
			   2 * out(i + 1).first().ncols()),
	  2 * out(i + 1).second());

      out(1).second() = std::max(out(2).first().ncols() * s - ima.ncols(),
				 out(2).first().nrows() * s - ima.nrows());

      trace::exiting("scribo::util::compute_sub_domains");
      return out;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_COMPUTE_SUB_DOMAINS_HH

