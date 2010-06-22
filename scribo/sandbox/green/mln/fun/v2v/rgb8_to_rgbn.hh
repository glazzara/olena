// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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

#ifndef  MLN_FUN_V2V_RGB8_TO_RGBN_HH
# define MLN_FUN_V2V_RGB8_TO_RGBN_HH

# include <mln/value/rgb8.hh>
# include <mln/value/rgb.hh>
# include <mln/core/contract.hh>

/// \file
///
/// \brief Convert rgb8 value to rgbn, n < 8.
///
/// The source implements the reduction of quantification for any size less 8.
///
/// The following sample is a typical use of the histogram.
///
/// #include <mln/accu/stat/histo3d_rgb.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/core/image/image3d.hh>
/// #include <mln/data/compute.hh>
/// #include <mln/data/transform.hh>
/// #include <mln/fun/v2v/rgb8_to_rgbn.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/value/rgb.hh>
/// #include <mln/value/rgb8.hh>
///
/// int main()
/// {
///   typedef mln::value::rgb8   t_rgb8;
///   typedef mln::value::rgb<7> t_rgb7;
///   mln::image2d<t_rgb8>       img_rgb8;
///   mln::image2d<t_rgb7>       img_rgb7;
///   mln::image3d<unsigned>     histo;
///
///   mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");
///   img_rgb7 =mln::data::transform(img_rgb8,mln::fun::v2v::rgb8_to_rgbn<7>());
///   histo = mln::data::compute(mln::accu::meta::stat::histo3d_rgb(),img_rgb7);
///
///   return 0;
/// }


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /// \brief Convert a rgb8 value to a rgn, n < 8.
      ///
      /// Param n defines the output quantification used for the transformation.
      ///
      /// \ingroup modfunv2v
      template <unsigned n>
      struct rgb8_to_rgbn : Function_v2v< rgb8_to_rgbn<n> >
      {
	typedef value::rgb8   argument;
	typedef value::rgb<n> result;

	/// \brief Convert a rgb8 value to a rgn, n < 8.
	///
	/// \param[in] v the rgb8 value to convert.
	///
	/// Conversion is done by computing the size by which we
	/// divide each rgb component.

	result operator()(const argument& c) const
	{
	  mln_precondition(8 > n);

	  unsigned size = pow(2,(8-n));
	  /*
	  std::cout << "c    : " << c << std::endl;
	  std::cout << "red  : " << c.red() << std::endl;
	  std::cout << "size : " << size << std::endl;
	  std::cout << "res  : " << (c.red() / size) << std::endl;
	  std::cout << "max  : " << (mln_max(mln::value::int_u<n>))<< std::endl;
	  */
	  result   res(c.red() / size, c.green() / size, c.blue() / size);

	  return res;
	}
      };

    }

  }

}

#endif // ! MLN_FUN_V2V_RGB8_TO_RGBN_HH
