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

#ifndef MLN_LEVEL_TRANSFORM_HH
# define MLN_LEVEL_TRANSFORM_HH

/*! \file mln/level/transform.hh
 *
 * \brief Transform the contents of an image into another one.
 *
 * \todo Clean this file + overload with pixel iterators.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>


// Specializations are in:
# include <mln/level/transform.spe.hh>


namespace mln
{

  namespace level
  {

    /*! Transform the image \p input through a function \p f to set
     *  the \p output image.
     *
     * \param[in] input The input image.
     * \param[in] f The function.
     * \param[out] output The result image.
     *
     * This routine runs: \n
     * for all p of \p input, \p output(p) = \p f( \p input(p) ).
     *
     * \pre \p output.domain >= \p input.domain
     */
    template <typename I, typename F, typename O>
    void transform(const Image<I>& input, const Function_v2v<F>& f, Image<O>& output);


    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
      transform(const Image<I>& input, const Function_v2v<F>& f);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename F, typename O>
	void transform_(const Image<I>& input_, const Function_v2v<F>& f_, Image<O>& output_)
	{
	  trace::entering("level::impl::generic::transform");

	  const I& input  = exact(input_);
	  const F& f      = exact(f_);
		O& output = exact(output_);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = f( input(p) );

	  trace::exiting("level::impl::generic::transform");
	}

      } // end of namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.

    template <typename I, typename F, typename O>
    void transform(const Image<I>& input, const Function_v2v<F>& f, Image<O>& output)
    {
      trace::entering("level::transform");

      mln_precondition(exact(output).domain() >= exact(input).domain());
      impl::transform_(mln_is_value_lowq(I)(),
		       exact(input), exact(f), exact(output));

      trace::exiting("level::transform");
    }


    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
      transform(const Image<I>& input, const Function_v2v<F>& f)
    {
      trace::entering("level::transform");

      mln_precondition(exact(input).has_data());
      mln_ch_value(I, mln_result(F)) output;
      initialize(output, input);
      transform_(input, f, output);

      trace::exiting("level::transform");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_TRANSFORM_HH
