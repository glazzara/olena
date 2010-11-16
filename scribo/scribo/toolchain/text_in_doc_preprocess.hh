// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <scribo/binarization/sauvola_ms.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/deskew.hh>


namespace scribo
{

  namespace toolchain
  {

    using namespace mln;

    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] enable_fg_bg Enable/Disable background removal.

      If \p enable_fg_bg is set to 'True' then a background removal is
      performed. Its parameter lambda is automatically set according
      to the input image size.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, bool enable_fg_bg);

    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] lambda Parameter to the background removal.

      If lambda is set to '0' no background removal is
      performed. Otherwise, a background removal is performed with the
      given \p lambda value.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda);

    /*! \brief Preprocess a document before looking for its content.

      \param[in] input An image.
      \param[in] lambda Parameter to the background removal.
      \param[in,out] fg The foreground layer of \p input.

      If lambda is set to '0' no background removal is
      performed. Otherwise, a background removal is performed with the
      given \p lambda value.

     */
    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda,
			   Image<I>& fg);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input_, bool enable_fg_bg)
    {
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      unsigned lambda = 0;
      if (enable_fg_bg)
	lambda = 1.2 * (input.nrows() + input.ncols());

      mln_ch_value(I,bool) output = text_in_doc_preprocess(input, lambda);

      return output;
    }

    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input, unsigned lambda)
    {
      I tmp;
      return text_in_doc_preprocess(input, lambda, tmp);
    }


    template <typename I>
    mln_ch_value(I,bool)
    text_in_doc_preprocess(const Image<I>& input_, unsigned lambda,
			   Image<I>& fg_)
    {
      trace::entering("scribo::toolchain::text_in_doc_preprocess");

      const I& input = exact(input_);
            I& fg = exact(fg_);
      mln_precondition(input.is_valid());

      mln_concrete(I) input_rgb = input;

      // Extract foreground
      if (lambda != 0)
      {
	std::cout << "Extracting foreground..." << std::endl;
	input_rgb = preprocessing::split_bg_fg(input, lambda, 32).second();
	fg = input_rgb;
      }

      // Convert to Gray level image.
      mln_ch_value(I,value::int_u8)
	input_gl = data::transform(input_rgb,
				   mln::fun::v2v::rgb_to_int_u<8>());


      // Deskewing
      std::cout << "Deskew if needed..." << std::endl;
      input_gl = preprocessing::deskew(input_gl);

      // Binarize foreground to use it in the processing chain.
      std::cout << "Binarizing foreground..." << std::endl;
      mln_ch_value(I,bool)
	input_bin = scribo::binarization::sauvola_ms(input_gl, 101, 3);

      trace::exiting("scribo::toolchain::text_in_doc_preprocess");
      return input_bin;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_DOC_PREPROCESS_HH

