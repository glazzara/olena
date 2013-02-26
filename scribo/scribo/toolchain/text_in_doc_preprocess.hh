// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_TOOLCHAIN_TEXT_IN_DOC_PREPROCESS_HH
# define SCRIBO_TOOLCHAIN_TEXT_IN_DOC_PREPROCESS_HH

/// \file
///
/// \brief Preprocess a document before looking for its content.


#include <mln/core/concept/image.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>

#include <scribo/binarization/sauvola_ms.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/deskew.hh>

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

namespace scribo
{

  namespace toolchain
  {

    using namespace mln;

    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] enable_fg_bg Enable/Disable background removal.
      \param[in] K Binarization threshold parameter. (Default 0.34)

      If \p enable_fg_bg is set to 'True' then a background removal is
      performed. Its parameter lambda is automatically set according
      to the input image size.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, bool enable_fg_bg,
			   unsigned lambda, double K, bool enable_deskew,
			   bool verbose);

    /*! \overload
      K is set to 0.34.
    */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, bool enable_fg_bg,
			   bool verbose);


    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] lambda Parameter to the background removal.

      If lambda is set to '0' no background removal is
      performed. Otherwise, a background removal is performed with the
      given \p lambda value.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda,
			   bool verbose);

    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] lambda Parameter to the background removal.
      \param[in] K Binarization threshold parameter. (Default 0.34)
      \param[in,out] fg The foreground layer of \p input.

      If lambda is set to '0' no background removal is
      performed. Otherwise, a background removal is performed with the
      given \p lambda value.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda,
			   double K, bool enable_fg_bg, Image<I>& fg,
			   bool enable_deskew, bool verbose);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, bool enable_fg_bg,
			   bool verbose)
    {
      return text_in_doc_preprocess(input, enable_fg_bg, 0.34, verbose);
    }


    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input_, bool enable_fg_bg,
			   unsigned lambda, double K, bool enable_deskew,
			   bool verbose)
    {
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      if (enable_fg_bg && lambda == 0)
	lambda = 1.2 * (input.nrows() + input.ncols());

      mln_concrete(I) tmp_fg;
      mln_ch_value(I,bool)
	output = text_in_doc_preprocess(input, lambda, K,
					enable_fg_bg, tmp_fg, enable_deskew, verbose);

      return output;
    }

    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda,
			   bool verbose)
    {
      I tmp;
      return text_in_doc_preprocess(input, lambda, 0.34, true, tmp, false, verbose);
    }


    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input_, unsigned lambda,
			   double K, bool enable_fg_bg, Image<I>& fg,
			   bool enable_deskew, bool verbose)
    {
      trace::entering("scribo::toolchain::text_in_doc_preprocess");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      internal::text_in_doc_preprocess_functor<I> f;

      // Setup functor.
      f.sauvola_K = K;
      f.enable_fg_extraction = enable_fg_bg;
      f.lambda = lambda;
      f.enable_deskew = enable_deskew;
      f.verbose = verbose;

      // Get results.
      mln_ch_value(I,bool) output = f(input);
      exact(fg) = f.fg;

      trace::exiting("scribo::toolchain::text_in_doc_preprocess");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_DOC_PREPROCESS_HH

