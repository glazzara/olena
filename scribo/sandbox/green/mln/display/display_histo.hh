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
///
/// This is the sample code to use the red/green visualization.
///
/// \fixme There is a lot of routines in display_histo.hh, but only
/// two of them have been call here. Stuff to complete.
///
/// #include <iostream>
/// #include <mln/accu/stat/histo3d_rgb.hh>
/// #include <mln/core/macros.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/core/image/image3d.hh>
/// #include <mln/data/compute.hh>
/// #include <mln/data/transform.hh>
/// #include <mln/display/display_histo.hh>
/// #include <mln/fun/v2v/rgb8_to_rgbn.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/io/pgm/save.hh>
/// #include <mln/io/ppm/save.hh>
/// #include <mln/literal/colors.hh>
/// #include <mln/value/rgb8.hh>
/// #include <mln/value/rgb.hh>
///
/// int main()
/// {
///  typedef mln::value::int_u8                          t_int_u8;
///  typedef mln::value::int_u<4>                        t_int_u4;
///  typedef mln::value::rgb8                            t_rgb8;
///  typedef mln::value::rgb<4>                          t_rgb4;
///  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
///  typedef mln::image2d<t_int_u4>                      t_image2d_int_u4;
///  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
///  typedef mln::image2d<t_rgb4>                        t_image2d_rgb4;
///  typedef mln::image2d<bool>                          t_image2d_bool;
///  typedef mln::image3d<unsigned>                      t_histo3d;
///  typedef mln::fun::v2v::rgb8_to_rgbn<4>              t_rgb8_to_rgb4;
///  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
///
///  t_image2d_rgb8                                   i0_input; // input rgb8
///  t_image2d_rgb4                                   i1_input; // input rgbn
///  t_histo3d                                        h1_input; // histo input
///  t_image2d_int_u8                                 p1_histo1;// histo proj1
///  t_image2d_rgb4                                   p1_histo2;// histo proj2
///  t_rgb4                                           red(mln::literal::red);
///
///  mln::io::ppm::load(i0_input, OLENA_IMG_PATH"/lena.ppm");
///
///  i1_input  = mln::data::transform(i0_input, t_rgb8_to_rgb4());
///  h1_input  = mln::data::compute(t_histo3d_fun(), i1_input);
///  p1_histo1 = mln::display::display_histo3d_unsigned(h1_input);
///  p1_histo2 = mln::display::display3_histo3d_unsigned(h1_input, red);
///
///  mln::io::pgm::save(p1_histo1, "proj1.pgm");
///  mln::io::ppm::save(p1_histo2, "proj2.ppm");
///
///  return 0;
/// }



namespace mln
{

  namespace display
  {

    /// \brief Allow the visualization of the density of the 3d histogram.
    ///
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \result return the projection of the histogram in 2d.
    ///
    /// The 3d histogram is projected in red/green space by
    /// accumulating around the the blue dimension (green and blue
    /// composantes are being correlated). In fact, we sum along the
    /// blue direction and take the log of value. Then, we stretch
    /// value to feet pgm format.
    image2d<value::int_u8>
    display_histo3d_unsigned(const image3d<unsigned>& histo);


    /// \brief Allow the visualization of the important color of the 3d histo.
    ///
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \parameter[in] ambiguous_color the color when no max is found.
    /// \result return the projection of the histogram in 2d.
    ///
    /// The 3d histogram is projected in red/green space. In the blue
    /// direction we check the max blue component. The position in
    /// this blue axis is returned in the projection image.
    template <unsigned n>
    image2d< value::int_u<n> >
    display2_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::int_u<n>    ambiguous_color);

    /// \brief Allow the visualization of the color of the 3d segmented histo.
    ///
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \parameter[in] label the label image to associate the result with.
    /// \parameter[in] ambiguous_color the color when no max is found.
    /// \result return the projection of the histogram in 2d.
    ///
    /// The 3d histogram is projected in red/green space. In the blue
    /// direction we check the max blue component. The position in
    /// this blue axis is associated to a segmented label and this label
    /// is returned in the projection image.
    template <unsigned n>
    image2d<value::label_8>
    display2_histo3d_unsigned(const image3d<unsigned>&       histo,
			      const image3d<value::label_8>& label,
			      const value::label_8           ambiguous_label);


    /// \brief Allow the visualization with color of the 3d histo.
    ///
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \parameter[in] ambiguous_color the color when no max is found.
    /// \result return the projection of the histogram in 2d.
    ///
    /// The 3d histogram is projected in red/green space. In the blue
    /// direction we check the max blue component. The position is used
    /// to build a rgb color and put it as a returned value in projection image.
    template <unsigned n>
    image2d< value::rgb<n> >
    display3_histo3d_unsigned(const image3d<unsigned>& histo,
			      const value::rgb<n>      ambiguous_color);


    /// \brief Allow the visualization with color of the 3d segmented histo.
    ///
    /// Direction r = 1
    /// Direction g = 2
    /// Direction b = 0
    ///
    /// \parameter[in] histo the histogram in 3d.
    /// \parameter[in] label the label image to associte the result with.
    /// \parameter[in] pal the color indexed list to associate to the label.
    /// \parameter[in] ambiguous_color the color when no max is found.
    /// \result return the projection of the histogram in 2d.
    ///
    /// The 3d histogram is projected in red/green space. In the blue
    /// direction we check the max blue component. The position is used
    /// to find the label associate to it and then the color associated to
    /// this label. That's the information we return in the projection image.
    template <unsigned n>
    image2d< value::rgb8 >
    display3_histo3d_unsigned(const image3d<unsigned>&               histo,
			      const image3d<value::label_8>&         label,
			      const util::array< algebra::vec<3,float> >& pal,
			      const value::rgb8              ambiguous_color);

#ifndef MLN_INCLUDE_ONLY

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
