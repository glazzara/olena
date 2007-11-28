// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_LEVEL_TRANSFORM_SPE_HH
# define MLN_LEVEL_TRANSFORM_SPE_HH

/*! \file mln/level/transform.spe.hh
 *
 * \brief Specializations for mln::level::transform.
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {


    namespace impl
    {


      namespace generic
      {
	template <typename I, typename F, typename O>
	void transform_(const Image<I>& input_, const Function_v2v<F>& f_, Image<O>& output_);
      }

      template <typename I, typename F, typename O>
      inline
      void transform_(mln::trait::image::quant::any, // general case
		      const Image<I>& input_, const Function_v2v<F>& f_, Image<O>& output_)
      {
	generic::transform_(input_, f_, output_);
      }


      template <typename I, typename F, typename O>
      inline
      void transform_(mln::trait::image::quant::low, // low quantization
		      const Image<I>& input_, const Function_v2v<F>& f_, Image<O>& output_)
      {
	trace::entering("level::impl::transform");

	const I& input  = exact(input_);
	const F& f      = exact(f_);
	      O& output = exact(output_);

	value::lut_vec<mln_vset(I), mln_result(F)> lut(input.values(), f);
	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = lut(input(p));

	trace::exiting("level::impl::transform");
      }


      // FIXME: Handle the cases of fastest images.

//       template <typename I, typename F, typename O>
//       void transform(metal::true_, // low quantization
// 		     const Image<I>& input_, const Function_v2v<F>& f_, Image<O>& output_)
//       {
// 	const I& input  = exact(input_);
// 	const F& f      = exact(f_);
// 	      O& output = exact(output_);

// 	value::lut_vec<mln_vset(I), mln_result(F)> lut(input.values(), f);
// 	mln_pixter(const I) pi(input); // FIXME
// 	mln_pixter(O) po(output);
// 	po.start();
// 	for_all(pi)
// 	  {
// 	    po.val() = lut(pi.val());
// 	    po.next();
// 	  }
//       }

    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_TRANSFORM_SPE_HH
