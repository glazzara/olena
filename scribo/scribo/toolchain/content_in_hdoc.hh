// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TOOLCHAIN_CONTENT_IN_HDOC_HH
# define SCRIBO_TOOLCHAIN_CONTENT_IN_HDOC_HH

/// \file
///
/// \brief Analyse a historical/degraded document.

# include <scribo/toolchain/internal/content_in_hdoc_functor.hh>

namespace scribo
{

  namespace toolchain
  {

    using namespace mln;

    /*! \brief Analyse and extract content in a historical/degraded
        document.

      \param[in] input A RGB image.
      \param[in] input_preproc A Binary image.
      \param[in] denoise Remove too small components (<= 2 pixels).
      \param[in] find_line_seps Enable/Disable lookup for
                                vertical/horizontal separators.
      \param[in] find_whitespace_seps Enable/Disable lookup for
                                      vertical alignments and
                                      whitespaces. Helps finding text
                                      layout.
      \param[in] enable_ocr Enable/Disable text recognition.
      \param[in] language Main language used in the input
                          document. (OCR settings)

      \return A document structure.

      \ingroup grptoolchaindocproc
     */
    template <typename I, typename J>
    document<mln_ch_value(I, def::lbl_type)>
    content_in_hdoc(const Image<I>& input, const Image<J>& input_preproc,
		   bool denoise,
		   bool find_line_seps = true,
		   bool find_whitespace_seps = true,
		   bool enable_ocr = true,
		   const std::string& language = std::string("eng"));


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename J>
    document<mln_ch_value(I, def::lbl_type)>
    content_in_hdoc(const Image<I>& input, const Image<J>& input_preproc,
		   bool denoise,
		   bool find_line_seps,
		   bool find_whitespace_seps,
		   bool enable_ocr,
		   const std::string& language)
    {
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(input_preproc).is_valid());

      internal::content_in_hdoc_functor<J> f("noname");
      f.enable_denoising = denoise;
      f.enable_line_seps = find_line_seps;
      f.enable_whitespace_seps = find_whitespace_seps;
      f.ocr_language = language;
      f.enable_ocr = enable_ocr;

      document<mln_ch_value(I, def::lbl_type)> doc = f(input, input_preproc);

      return doc;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_CONTENT_IN_HDOC_HH

