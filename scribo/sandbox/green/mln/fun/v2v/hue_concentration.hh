// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_FUN_V2V_HUE_CONCENTRATION_HH
# define MLN_FUN_V2V_HUE_CONCENTRATION_HH

# include <mln/core/image/image1d.hh>
# include <mln/math/abs.hh>
# include <mln/opt/at.hh>
# include <mln/value/hsv.hh>
# include <mln/value/rgb8.hh>

/// \file
///
/// This is the code for building hue_concentration_map.

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /// \brief internal method for detecting the histogram peak.
      ///
      /// \param[in] hue_histo the histogram of hue.
      ///
      /// \return the seed of the peek.
      unsigned peak_histo(const mln::image1d<unsigned>& hue_histo);

      struct hue_concentration : public Function_v2v< hue_concentration >
      {
	typedef float result;

	/// \brief Gibe the distance map between actual hue and the peak.
	float operator()(const float hue) const;

	hue_concentration(const mln::image1d<unsigned>& hue_histo)
	{
	  peak = peak_histo(hue_histo);
	}

	unsigned peak;
      };


# ifndef MLN_INCLUDE_ONLY


      unsigned peak_histo(const mln::image1d<unsigned>& hue_histo)
      {
	mln_precondition(hue_histo.is_valid());

	// Find the peak of the histogram
	unsigned v_max = mln::opt::at(hue_histo, 0);
	short    p_max = 0;

	mln_piter_(mln::image1d<unsigned>) p(hue_histo.domain());

	for_all(p)
	{
	  if (v_max < hue_histo(p))
	  {
	    v_max = hue_histo(p);
	    p_max = p.ind();
	  }
	}

	return p_max;
      }


      float hue_concentration::operator()(float hue) const
      {
	return mln::math::abs(peak - hue);
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_HUE_CONCENTRATION_HH
