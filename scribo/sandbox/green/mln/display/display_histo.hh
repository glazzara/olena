// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DISPLAY_DISPLAY_HISTO_HH
# define MLN_DISPLAY_DISPLAY_HISTO_HH

# include <mln/accu/math/sum.hh>
# include <mln/algebra/vec.hh>
# include <mln/data/stretch.hh>
# include <mln/display/project_histo.hh>
# include <mln/fun/v2v/log.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/label_8.hh>
# include <mln/util/array.hh>


/// \file
///
/// \brief Allow the complete visualization of a 3d histogram by projection.
///
/// The 3d histogram is projected in red/green space by accumulating around the
/// the blue dimension (green and blue composantes are being correlated). In
/// fact, we sum in along the blue direction. Then, we stretch value to feet
/// pgm format.


namespace mln
{

  namespace display
  {

    // Forward declaration.
    image2d<value::int_u8>
    display_histo3d_unsigned(const image3d<unsigned>& histo);

    template <unsigned n>
    image2d< value::int_u<n> >
    display2_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::int_u<n>    ambiguous_color);

    template <unsigned n>
    image2d<value::label_8>
    display2_histo3d_unsigned(const image3d<unsigned>&       histo,
			      const image3d<value::label_8>& label,
			      const value::label_8           ambiguous_label);

    template <unsigned n>
    image2d< value::rgb<n> >
    display3_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::rgb<n>      ambiguous_color);

    template <unsigned n>
    image2d< value::rgb8 >
    display3_histo3d_unsigned(const image3d<unsigned>&               histo,
			      const image3d<value::label_8>&         label,
			      const util::array< algebra::vec<3,float> >& pal,
			      const value::rgb8              ambiguous_color);

#ifndef MLN_INCLUDE_ONLY

    /// \brief Allow the visualization of a 3d histogram by projection.
    ///
    /// The 3d histogram is projected in red/green space by
    /// accumulating around the the blue dimension (green and blue
    /// composantes are being correlated). In fact, we sum along
    /// the blue direction. Then, we stretch value to feet pgm
    /// format.
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \result return a equivalent 2d image.

    // FIXME : display_shape [in int_u8]
    image2d<value::int_u8>
    display_histo3d_unsigned(const image3d<unsigned>& histo)
    {
      typedef accu::math::sum<unsigned,unsigned> t_sum;
      typedef value::int_u8                      t_int_u8;
      typedef fun::v2v::log<float>               t_log;

      image2d<unsigned> proj     = project_histo<t_sum,0>(histo);
      image2d<t_int_u8> proj_int = data::stretch(t_int_u8(),
						 data::transform(proj,
								 t_log()));
      return proj_int;
    }

    // FIXME : display_color [in int_un]
    template <unsigned n>
    image2d< value::int_u<n> >
    display2_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::int_u<n>    ambiguous_color)
    {
      image2d< value::int_u<n> > proj = project2_histo<n,0>(histo,
							    ambiguous_color);

      return proj;
    }

    // FIXME : display_label [in label]
    template <unsigned n>
    image2d<value::label_8>
    display2_histo3d_unsigned(const image3d<unsigned>&       histo,
			      const image3d<value::label_8>& label,
			      const value::label_8           ambiguous_label)
    {
      image2d<value::label_8> proj = project2_histo<n,0>(histo,
							 label,
							 ambiguous_label);

      return proj;
    }

    // FIXME : display_color [in color]
    template <unsigned n>
    image2d< value::rgb<n> >
    display3_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::rgb<n>      ambiguous_color)
    {
      image2d< value::rgb<n> > proj = project3_histo<n,0>(histo,
							  ambiguous_color);

      return proj;
    }


    // FIXME : display_label [in color]
    template <unsigned n>
    image2d< value::rgb8 >
    display3_histo3d_unsigned(const image3d<unsigned>&              histo,
			      const image3d<value::label_8>&        label,
			      const util::array<algebra::vec<3,float> >& pal,
			      const value::rgb8             ambiguous_color)
    {
      image2d< value::rgb8 > proj = project3_histo<n,0>(histo,
							  label,
							  pal,
							  ambiguous_color);

      return proj;
    }


#endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_DISPLAY_DISPLAY_HISTO_HH
