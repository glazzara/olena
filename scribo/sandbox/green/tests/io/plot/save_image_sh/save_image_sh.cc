// Copyright (C) 2007,2008,2009,2010 EPITA LRDE
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

/// \file
///
/// \brief This source manages unitary testing on gnuplot shell export library.
///
/// Tests are performed in various image format, such as image1d, image2d<bool>,
/// image2d<int_u8>, image2d<int_u16>, image2d<int_s8>, image2d<int_s16>,
/// image2d<float>, image2d<double>, image2d<rgb8>, image2d<rgb16>,
/// image2d<hsl_f>, image2d<hsl_d>, image2d<hsi_f>, image2d<hsi_d>, image3d.

#include <iostream>
#include <limits.h>

#include <mln/img_path.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb16.hh>
#include <mln/value/hsl.hh>
#include <mln/value/hsi.hh>
#include <mln/util/array.hh>
#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/fun/v2v/rgb_to_hsl.hh>
#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgms/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/plot/save_image_sh.hh>
#include <mln/io/plot/load.hh>

#include <mln/data/transform.hh>
#include <mln/literal/colors.hh>
#include <mln/debug/println.hh>


/// image1d<int_u8>.
/// \{
/// \brief Test saving a grayscale image1d.
///
/// As working on the image1d interface is something extremely rare, we need
/// to use the plot interface to first load some datas and then export them.

mln::image1d<mln::value::int_u8>
convert_array_to_image1d(const mln::util::array<mln::value::int_u8>& array)
{
  typedef mln::value::int_u8       int_u8;
  typedef mln::image1d<int_u8>     image1d;
  typedef mln::util::array<int_u8> array1d;

  image1d result(array.nelements());

  mln_piter_(image1d)     p(result.domain());
  mln_eiter_(array1d)     e(array);

  for_all_2(e, p)
  {
    result(p) = e.element();
  }

  return result;
}

void test_save_image_sh_1d()
{
  typedef mln::value::int_u8       int_u8;
  typedef mln::image1d<int_u8>     image1d;
  typedef mln::util::array<int_u8> array1d;

  image1d img;
  array1d array;

  mln::io::plot::load(array, OLENA_IMG_PATH"/fly.plot");
  img = convert_array_to_image1d(array);

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img, "/dev/null/fly.sh");

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img, "fly1d.sh");

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image 1d : ok" << std::endl;
}

/// \}

/// image2d<bool>.
/// \{
/// \brief Test saving an image2d<bool>.
///
/// Boolean image2d are extremly convenient for storing mask. We have a lots
/// of example in milena.

void test_save_image2d_bool()
{
  typedef mln::image2d<bool>   image2d_bool;
  const   char*                null_lena = "/dev/null/lena2d_bool.sh";
  const   char*                lena      = "lena2d_bool.sh";

  image2d_bool img_bool;

  mln::io::pbm::load(img_bool, OLENA_IMG_PATH"/lena.pbm");

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_bool, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_bool, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<bool>: ok" << std::endl;
}

/// \}

/// image2d<int_u8>.
/// \{
/// \brief Test saving a grayscale image2d.
///
/// Grayscale image2d are very popular in image processing. It's the test we do
/// not fail!

void test_save_image2d_int_u8()
{
  typedef mln::value::int_u8   int_u8;
  typedef mln::image2d<int_u8> image2d_int_u8;
  const   char*                null_lena = "/dev/null/lena2d_int_u8.sh";
  const   char*                lena      = "lena2d_int_u8.sh";

  image2d_int_u8 img_int_u8;

  mln::io::pgm::load(img_int_u8, OLENA_IMG_PATH"/lena.pgm");

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_int_u8, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_int_u8, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<int_u8>: ok" << std::endl;
}

/// \}

/// image2d<int_u16>.
/// \{
/// \brief Test saving an extended grayscale image2d.
///
/// This kind of image are not so current. But for medical use, we can get now,
/// some dicom in 12 bits and new generation bring us 16 bits. As 16 bits is a
/// strange format for image processing people, there is not a common
/// acceptation for the underline format. Xv or imageMagick doesn't agree for
/// the representation of the file.


struct int_u16_to_int_u8 : mln::Function_v2v< int_u16_to_int_u8 >
{
  typedef mln::value::int_u16   argument;
  typedef mln::value::int_u8    result;

  result operator()(const argument& c) const
  {
    result res(c / 256);

    return res;
  }
};

void test_save_image2d_int_u16()
{
  typedef mln::value::int_u8    int_u8;
  typedef mln::value::int_u16   int_u16;
  typedef mln::image2d<int_u8>  image2d_int_u8;
  typedef mln::image2d<int_u16> image2d_int_u16;
  const   char*                 null_lena = "/dev/null/lena2d_int_u16.sh";
  const   char*                 lena      = "lena2d_int_u16.sh";

  image2d_int_u8  img_int_u8;
  image2d_int_u16 img_int_u16;

  mln::io::pgm::load(img_int_u16, OLENA_IMG_PATH"/lena_16_gnuplot.pgm");
  //img_int_u8 = mln::data::transform(img_int_u16, int_u16_to_int_u8());
  //mln::io::pgm::save(img_int_u8, "lena_8.pgm");

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_int_u16, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_int_u16, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<int_u16> : ok" << std::endl;
}

/// \}

/// image2d<int_s8>.
/// \{
/// \brief Test saving an signed image2d.
///
/// This kind of image are very artificial. It is given for exhaustivity of
/// testing. In fact, this kind of image traduces some step in a computing
/// chain.


struct int_u8_to_int_s8 : mln::Function_v2v< int_u8_to_int_s8 >
{
  typedef mln::value::int_u8    argument;
  typedef mln::value::int_s8    result;

  result operator()(const argument& c) const
  {
    argument tmp_int_u8;
    result   tmp_int_s8;
    result   res;

    // Note : -127 <= int_s8 <= +127
    if (UCHAR_MAX == c)
    {
      res = SCHAR_MAX;
    }
    else if (static_cast<unsigned>(SCHAR_MAX) < c)
    {
      tmp_int_u8 = c - SCHAR_MAX;
      res        = static_cast<mln::value::int_s8>(tmp_int_u8);
    }
    else
    {
      tmp_int_s8 = static_cast<mln::value::int_s8>(c) - SCHAR_MAX;
      res        = tmp_int_s8;
    }

    return res;
  }
};


void test_save_image2d_int_s8()
{
  typedef mln::value::int_s8   int_s8;
  typedef mln::value::int_u8   int_u8;
  typedef mln::image2d<int_s8> image2d_int_s8;
  typedef mln::image2d<int_u8> image2d_int_u8;
  const   char*                null_lena = "/dev/null/lena2d_int_s8.sh";
  const   char*                lena      = "lena2d_int_s8.sh";

  image2d_int_s8 img_int_s8;
  image2d_int_u8 img_int_u8;

  mln::io::pgm::load(img_int_u8, OLENA_IMG_PATH"/lena.pgm");
  img_int_s8 = mln::data::transform(img_int_u8, int_u8_to_int_s8());

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_int_s8, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_int_s8, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<int_s8>: ok" << std::endl;
}

/// \}

/// image2d<int_s16>.
/// \{
/// \brief Test saving an extended image2d.
///
/// This kind of image are very artificial. It is given for exhaustivity of
/// testing. In fact, this kind of image traduces some step in a computing
/// chain.

struct int_u16_to_int_s16 : mln::Function_v2v< int_u16_to_int_s16 >
{
  typedef mln::value::int_u16    argument;
  typedef mln::value::int_s16    result;

  result operator()(const argument& c) const
  {
    argument tmp_int_u16;
    result   tmp_int_s16;
    result   res;

    if (USHRT_MAX == c)
    {
      res = SHRT_MAX;
    }
    else if (static_cast<unsigned>(SHRT_MAX) < c)
    {
      tmp_int_u16 = c - SHRT_MAX;
      res        = static_cast<mln::value::int_s16>(tmp_int_u16);
    }
    else
    {
      tmp_int_s16 = static_cast<mln::value::int_s16>(c) - SHRT_MAX;
      res         = tmp_int_s16;
    }

    return res;
  }
};

void test_save_image2d_int_s16()
{
  typedef mln::value::int_u16   int_u16;
  typedef mln::value::int_s16   int_s16;
  typedef mln::image2d<int_s16> image2d_int_s16;
  typedef mln::image2d<int_u16> image2d_int_u16;
  const   char*                 null_lena = "/dev/null/lena2d_int_s16.sh";
  const   char*                 lena      = "lena2d_int_s16.sh";

  image2d_int_s16 img_int_s16;
  image2d_int_u16 img_int_u16;

  mln::io::pgm::load(img_int_u16, OLENA_IMG_PATH"/lena_16_gnuplot.pgm");
  img_int_s16 = mln::data::transform(img_int_u16, int_u16_to_int_s16());

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_int_s16, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_int_s16, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<int_s16> : ok" << std::endl;
}

/// \}

/// image2d<float>.
/// \{
/// \brief Test saving a float image2d.
///
/// This kind of image is a common way to save some attributes localized on
/// pixels.

struct int_u8_to_float : mln::Function_v2v< int_u8_to_float >
{
  typedef mln::value::int_u8   argument;
  typedef float                result;

  result operator()(const argument& c) const
  {
    result res = static_cast<float>(c)/3;

    return res;
  }
};

void test_save_image2d_float()
{
  typedef mln::value::int_u8   int_u8;
  typedef mln::image2d<float>  image2d_float;
  typedef mln::image2d<int_u8> image2d_int_u8;
  const   char*                null_lena = "/dev/null/lena2d_float.sh";
  const   char*                lena      = "lena2d_float.sh";

  image2d_int_u8 img_int_u8;
  image2d_float  img_float;

  mln::io::pgm::load(img_int_u8, OLENA_IMG_PATH"/lena.pgm");
  img_float = mln::data::transform(img_int_u8, int_u8_to_float());

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_float, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_float, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<float> : ok" << std::endl;
}

/// \}

/// image2d<double>.
/// \{
/// \brief Test saving a double image2d.
///
/// This kind of image is a common way to save some attributes localized on
/// pixels.

struct int_u16_to_double : mln::Function_v2v< int_u16_to_double >
{
  typedef mln::value::int_u16  argument;
  typedef double               result;

  result operator()(const argument& c) const
  {
    result res = static_cast<float>(c)/256;

    return res;
  }
};

void test_save_image2d_double()
{
  typedef mln::value::int_u16   int_u16;
  typedef mln::image2d<double>  image2d_double;
  typedef mln::image2d<int_u16> image2d_int_u16;
  const   char*                 null_lena = "/dev/null/lena2d_double.sh";
  const   char*                 lena      = "lena2d_double.sh";

  image2d_int_u16 img_int_u16;
  image2d_double  img_double;

  mln::io::pgm::load(img_int_u16, OLENA_IMG_PATH"/lena_16_gnuplot.pgm");
  img_double = mln::data::transform(img_int_u16, int_u16_to_double());

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_double, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_double, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<double> : ok" << std::endl;
}

/// \}

/// image2d<rgb8>.
/// \{
/// \brief Test saving a RGB color image2d.
///
/// This kind of format is extremely popular in image, and in photography.

void test_save_image2d_rgb8()
{
  typedef mln::value::rgb8      rgb8;
  typedef mln::image2d<rgb8>    image2d_rgb8;
  const   char*                 null_lena = "/dev/null/lena2d_rgb8.sh";
  const   char*                 lena      = "lena2d_rgb8.sh";

  image2d_rgb8 img_rgb8;

  mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_rgb8, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_rgb8, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<rgb8> : ok" << std::endl;
}

/// \}

/// image2d<rgb16>.
/// \{
/// \brief Test saving a 16 bits RGB color image2d.
///
/// As this kind of image is less popular, the library enable this export by
/// the will of exhaustivity.

struct rgb16_to_int_u16 : mln::Function_v2v< rgb16_to_int_u16 >
{
  typedef mln::value::rgb16   argument;
  typedef mln::value::int_u16 result;


  result operator()(const argument& c) const
  {
    result res((c.red() + c.green() + c.blue())/3);

    return res;
  }
};

void test_save_image2d_rgb16()
{
  typedef mln::value::rgb16      rgb16;
  typedef mln::value::int_u16    int_u16;
  typedef mln::image2d<rgb16>    image2d_rgb16;
  typedef mln::image2d<int_u16>  image2d_int_u16;
  const   char*                  null_lena = "/dev/null/lena2d_rgb16.sh";
  const   char*                  lena      = "lena2d_rgb16.sh";
  //const   char*                  lena2     = "lena2d_int_u16.sh";


  image2d_rgb16   img_rgb16;
  image2d_int_u16 img_int_u16;

  mln::io::ppm::load(img_rgb16, OLENA_IMG_PATH"/lena_16.ppm");
  //img_int_u16 = mln::data::transform(img_rgb16, rgb16_to_int_u16());
  //mln::io::pgm::save(img_int_u16, "lena_16.pgm");

  // Save it were it is impossible to do it !!
  bool ref_false=mln::io::plot::save_image_sh(img_rgb16, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_rgb16, lena);
  //  mln::io::plot::save_image_sh(img_int_u16, lena2);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<rgb16> : ok" << std::endl;
}

/// \}

/// image2d<hsl_f>.
/// \{
/// \brief Test saving a HSL float color image2d.
///
/// This kind of image allow to show the power of gnuplot. Raw data
/// are hsl, then hsl equations are integrated in the gnuplot shell
/// file. So decoding needs more time.

void test_save_image2d_hsl_f()
{
  typedef mln::value::hsl_f                   hsl_f;
  typedef mln::value::rgb8                    rgb8;
  typedef mln::image2d<hsl_f>                 image2d_hsl_f;
  typedef mln::image2d<rgb8>                  image2d_rgb8;
  typedef mln::fun::v2v::f_rgb_to_hsl_<hsl_f> rgb_to_hsl;

  const   char* null_lena = "/dev/null/lena2d_hsl_f.sh";
  const   char* lena      = "lena2d_hsl_f.sh";
  image2d_hsl_f img_hsl_f;
  image2d_rgb8  img_rgb8;

  mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");
  //mln::data::fill(img_rgb8, mln::literal::red);
  //mln::data::fill(img_rgb8, mln::literal::green);
  //mln::data::fill(img_rgb8, mln::literal::blue);
  img_hsl_f = mln::data::transform(img_rgb8, rgb_to_hsl());


  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_hsl_f, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_hsl_f, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<hsl_f> : ok" << std::endl;
}

/// \}

/// image2d<hsl_d>.
/// \{
/// \brief Test saving a HSL double color image2d.
///
/// This kind of image allow to show the power of gnuplot. Raw data
/// are hsl, then hsl equations are integrated in the gnuplot shell
/// file. So decoding needs more time.

void test_save_image2d_hsl_d()
{
  typedef mln::value::hsl_d                   hsl_d;
  typedef mln::value::rgb16                   rgb16;
  typedef mln::image2d<hsl_d>                 image2d_hsl_d;
  typedef mln::image2d<rgb16>                 image2d_rgb16;
  typedef mln::fun::v2v::f_rgb_to_hsl_<hsl_d> rgb_to_hsl;

  const   char* null_lena = "/dev/null/lena2d_hsl_d.sh";
  const   char* lena      = "lena2d_hsl_d.sh";
  image2d_hsl_d img_hsl_d;
  image2d_rgb16 img_rgb16;

  mln::io::ppm::load(img_rgb16, OLENA_IMG_PATH"/lena_16.ppm");
  //mln::data::fill(img_rgb16, mln::literal::red);
  //mln::data::fill(img_rgb16, mln::literal::green);
  //mln::data::fill(img_rgb16, mln::literal::blue);
  img_hsl_d = mln::data::transform(img_rgb16, rgb_to_hsl());


  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_hsl_d, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_hsl_d, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<hsl_d> : ok" << std::endl;
}

/// \}

/// image2d<hsi_f>.
/// \{
/// \brief Test saving a HSI float color image2d.
///
/// This kind of image allow to show the power of gnuplot. Raw data
/// are hsi, then hsi equations are integrated in the gnuplot shell
/// file. So decoding needs more time.

void test_save_image2d_hsi_f()
{
  typedef mln::value::hsi_f                   hsi_f;
  typedef mln::value::rgb8                    rgb8;
  typedef mln::image2d<hsi_f>                 image2d_hsi_f;
  typedef mln::image2d<rgb8>                  image2d_rgb8;
  typedef mln::fun::v2v::f_rgb_to_hsi_<hsi_f> rgb_to_hsi;

  const   char* null_lena = "/dev/null/lena2d_hsi_f.sh";
  const   char* lena      = "lena2d_hsi_f.sh";
  image2d_hsi_f img_hsi_f;
  image2d_rgb8  img_rgb8;

  mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");
  //mln::data::fill(img_rgb8, mln::literal::red);
  //mln::data::fill(img_rgb8, mln::literal::green);
  //mln::data::fill(img_rgb8, mln::literal::blue);
  img_hsi_f = mln::data::transform(img_rgb8, rgb_to_hsi());


  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_hsi_f, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_hsi_f, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<hsi_f> : ok" << std::endl;
}

/// \}

/// image2d<hsi_d>.
/// \{
/// \brief Test saving a HSI double color image2d.
///
/// This kind of image allow to show the power of gnuplot. Raw data
/// are hsi, then hsi equations are integrated in the gnuplot shell
/// file. So decoding needs more time.

void test_save_image2d_hsi_d()
{
  typedef mln::value::hsi_d                   hsi_d;
  typedef mln::value::rgb16                   rgb16;
  typedef mln::image2d<hsi_d>                 image2d_hsi_d;
  typedef mln::image2d<rgb16>                 image2d_rgb16;
  typedef mln::fun::v2v::f_rgb_to_hsi_<hsi_d> rgb_to_hsi;

  const   char* null_lena = "/dev/null/lena2d_hsi_d.sh";
  const   char* lena      = "lena2d_hsi_d.sh";
  image2d_hsi_d img_hsi_d;
  image2d_rgb16 img_rgb16;

  mln::io::ppm::load(img_rgb16, OLENA_IMG_PATH"/lena_16.ppm");
  //mln::data::fill(img_rgb16, mln::literal::red);
  //mln::data::fill(img_rgb16, mln::literal::green);
  //mln::data::fill(img_rgb16, mln::literal::blue);
  img_hsi_d = mln::data::transform(img_rgb16, rgb_to_hsi());


  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img_hsi_d, null_lena);

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img_hsi_d, lena);

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image2d<hsi_d> : ok" << std::endl;
}

/// \}

/// image3d<int_8>.
/// \{
/// \brief Test saving a grayscale image3d.
///
/// It seems important to be able to export in 3d, but it's not so popular
/// in the team. We try to simulate this functionnality.
///
/// \fixme The data visualization is very hard in 3d compact with gnuplot.

void test_save_image_sh_3d()
{
  typedef mln::value::int_u8            int_u8;
  typedef mln::image3d<int_u8>          image3d;
  typedef mln::util::array<std::string> array1d;

  image3d img;
  array1d array;

  for (unsigned i = 0; i < 5; i++)
    array.append(OLENA_IMG_PATH"/fly.pgm");

  mln::io::pgms::load(img, array);

  // Save it were it is impossible to do it !!
  bool ref_false  = mln::io::plot::save_image_sh(img, "/dev/null/fly3d.sh");

  // Save it where we have the full permissions.
  bool ref_true = mln::io::plot::save_image_sh(img, "fly3d.sh");

  mln_assertion(true  == ref_true);
  mln_assertion(false == ref_false);
  std::cout << "save image 3d : ok" << std::endl;
}

/// \}


int main()
{
  test_save_image_sh_1d();

  test_save_image2d_bool();

  test_save_image2d_int_u8();
  test_save_image2d_int_u16();

  test_save_image2d_int_s8();
  test_save_image2d_int_s16();

  test_save_image2d_float();
  test_save_image2d_double();

  test_save_image2d_rgb8();
  test_save_image2d_rgb16();

  test_save_image2d_hsl_f();
  test_save_image2d_hsl_d();

  test_save_image2d_hsi_f();
  test_save_image2d_hsi_d();

  test_save_image_sh_3d();

  return 0;
}
