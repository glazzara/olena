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

#ifndef SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH
# define SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH

/// \file
///
/// Extract text from a document.

# include <scribo/toolchain/internal/text_in_doc_functor.hh>

namespace scribo
{

  namespace toolchain
  {

    using namespace mln;


    template <typename I>
    line_set<mln_ch_value(I, def::lbl_type)>
    text_in_doc(const Image<I>& input, bool denoise,
		const std::string& language = std::string("eng"),
		bool find_line_seps = true,
		bool find_whitespace_seps = true,
		bool verbose = false);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    line_set<mln_ch_value(I, def::lbl_type)>
    text_in_doc(const Image<I>& input, bool denoise,
		const std::string& language = std::string("eng"),
		bool find_line_seps = true,
		bool find_whitespace_seps = true,
		bool verbose = false)
    {
      internal::text_in_doc_functor<I> f;
      f.enable_denoising = denoise;
      f.enable_line_seps = find_line_seps;
      f.enable_whitespace_seps = find_whitespace_seps;
      f.ocr_language = language;
      f.verbose = verbose;

      line_set<mln_ch_value(I, def::lbl_type)> lines = f(input);

      return lines;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH

