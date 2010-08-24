// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_P2B_ACHROMATIC_HH
# define MLN_FUN_P2B_ACHROMATIC_HH

/// \file
///
/// In the vectorial image context (as RGB, HSL, HSV), compare one component
/// to a specific value.

# include <mln/core/alias/point2d.hh>
# include <mln/core/concept/function.hh>
# include <mln/trait/value/comp.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {
      /// \brief Functor that compare the i-th component of a value.
      // V is for the type of the value received
      // i is the ith component to select
      template <typename T_rgb>
      struct achromatic : public Function_v2b< achromatic<T_rgb> >
      {
	typedef bool result;
	bool operator()(const point2d& p) const;

	achromatic(const image2d<T_rgb>& img, const float threshold);

	const float           threshold_;
	const image2d<T_rgb>& img_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T_rgb>
      achromatic<T_rgb>::achromatic(const image2d<T_rgb>& img,
				    const float           threshold)
	: threshold_(threshold), img_(img)
      {
      }

      template <typename T_rgb>
      bool achromatic<T_rgb>::operator()(const point2d& p) const
      {
	typedef typename T_rgb::red_t   t_red;
	typedef typename T_rgb::green_t t_green;
	typedef typename T_rgb::blue_t  t_blue;

	const T_rgb rgb       = img_(p);

	// To obtain red between 0 and 1
	const float max_red   = mln_max(t_red);
	const float min_red   = mln_min(t_red);
	const float red       = (rgb.red() - min_red)/(max_red - min_red);

	// To obtain green between 0 and 1
	const float max_green = mln_max(t_green);
	const float min_green = mln_min(t_green);
	const float green     = (rgb.green()-min_green)/(max_green-min_green);

	// To obtain blue between 0 and 1
	const float max_blue  = mln_max(t_blue);
	const float min_blue  = mln_min(t_blue);
	const float blue      = (rgb.blue()-min_blue)/(max_blue-min_blue);

	bool result = (threshold_ > math::abs(red - green) &&
		       threshold_ > math::abs(red - blue) &&
		       threshold_ > math::abs(green - blue));

	return result;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_ACHROMATIC_HH
