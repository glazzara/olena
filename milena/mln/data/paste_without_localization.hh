// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH
# define MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH

/// \file mln/data/paste_without_localization.hh
///
/// Paste the contents of an image into another one without taking
/// into account the localization of sites.

# include <mln/core/concept/image.hh>



namespace mln
{

  namespace data
  {

    /// \brief Paste the contents of image \p input into the image \p
    /// output without taking into account the localization of sites.
    ///
    /// \param[in] input The input image providing pixels values.
    /// \param[in,out] output The image in which values are
    /// assigned.
    //
    template <typename I, typename J>
    void paste_without_localization(const Image<I>& input, Image<J>& output);



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename J>
    inline
    void paste_without_localization(const Image<I>& input_, Image<J>& output_)
    {
      trace::entering("data::paste_without_localization");

      mlc_converts_to(mln_value(I), mln_value(J))::check();

      const I& input = exact(input_);
      J& output = exact(output_);
    
      mln_fwd_piter(I) pi(input.domain());
      pi.start();
      mln_fwd_piter(J) po(output.domain());
      po.start();
      while (pi.is_valid() && po.is_valid())
	{
	  output(po) = input(pi);
	  pi.next();
	  po.next();
	}

      trace::exiting("data::paste_without_localization");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln



#endif // ! MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH
