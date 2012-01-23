// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_TIFF_LOAD_HH
# define MLN_IO_TIFF_LOAD_HH


/// \file
///
/// Load TIFF images to Milena images.
///
/// \todo Add support for several tiles.
/// \todo Use ReadScanline instead of ReadRGBAImage in order to avoid a
///	  dispatch and share the same code whatever is the value type.


# include <iostream>
# include <fstream>
# include <tiffio.h>

# include <mln/core/concept/image.hh>
# include <mln/value/rgb8.hh>



namespace mln
{

  namespace io
  {

    namespace tiff
    {


      /*! \brief Load a TIFF image to a Milena image.
       *
       * \ingroup iotiff
       */
      template <typename I>
      void load(Image<I>& ima_, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	inline
	point2d ij2rc_1(int i, int j, int ni_1, int nj_1)
	{
	  (void) nj_1;
	  return point2d(ni_1 - i, j);
	}

	inline
	point2d ij2rc_2(int i, int j, int ni_1, int nj_1)
	{
	  return point2d(ni_1 - i, nj_1 - j);
	}

	inline
	point2d ij2rc_3(int i, int j, int ni_1, int nj_1)
	{
	  (void) ni_1;
	  return point2d(i, nj_1 - j);
	}

	inline
	point2d ij2rc_4(int i, int j, int ni_1, int nj_1)
	{
	  (void) ni_1;
	  (void) nj_1;
	  return point2d(i, j);
	}

	inline
	point2d ij2rc_5(int i, int j, int ni_1, int nj_1)
	{
	  (void) nj_1;
	  return point2d(j, ni_1 - i);
	}

	inline
	point2d ij2rc_6(int i, int j, int ni_1, int nj_1)
	{
	  return point2d(nj_1 - j, ni_1 - i);
	}

	inline
	point2d ij2rc_7(int i, int j, int ni_1, int nj_1)
	{
	  (void) ni_1;
	  return point2d(nj_1 - j, i);
	}

	inline
	point2d ij2rc_8(int i, int j, int ni_1, int nj_1)
	{
	  (void) ni_1;
	  (void) nj_1;
	  return point2d(j, i);
	}



	template <typename I>
        inline
        I load_header(TIFF *file)
	{
	  uint32 width, height;

	  TIFFGetField(file, TIFFTAG_IMAGEWIDTH, &width);
	  TIFFGetField(file, TIFFTAG_IMAGELENGTH, &height);

	  mln_concrete(I) new_ima(height, width, 0);

	  return new_ima;
	}


	template <typename I>
        inline
        void load_data_rgb8(I& ima, TIFF *file)
	{
	  uint16 bits_per_sample, samples_per_pixel;

	  TIFFGetField(file, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	  TIFFGetField(file, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);

	  uint16 data_size = bits_per_sample * samples_per_pixel;
	  if (data_size != 24 && data_size != 32)
	  {
	    std::cout << "Trying to load a non color TIFF "
		      << "image into a color Milena image." << std::endl;
	    abort();
	  }

	  uint32 npixels = ima.ncols() * ima.nrows();
	  uint32 *raster = (uint32 *) _TIFFmalloc(npixels * sizeof (uint32));

	  if (!TIFFReadRGBAImage(file, ima.ncols(), ima.nrows(), raster, 0))
	  {
	    std::cout << "Error while reading the image file. Is it corrupted?"
		      << std::endl;
	    abort();
	  }

	  uint16 orientation;
	  TIFFGetField(file, TIFFTAG_ORIENTATION, &orientation);

	  typedef point2d (*fun_t)(int, int, int, int);
	  fun_t funs[] = { 0, ij2rc_1, ij2rc_2, ij2rc_3, ij2rc_4,
			      ij2rc_5, ij2rc_6, ij2rc_7, ij2rc_8 };
	  fun_t fun = funs[orientation];

	  int ni_1, nj_1;
	  unsigned idx = 0;

	  if (orientation <= 4)
	    {
	      ni_1 = ima.nrows() - 1;
	      nj_1 = ima.ncols() - 1;
	      for (int i = 0; i <= ni_1; ++i)
		for (int j = 0; j <= nj_1; ++j)
		  {
		    value::rgb8 v;

		    v.red() = (unsigned char) TIFFGetR(raster[idx]);
		    v.green() = (unsigned char) TIFFGetG(raster[idx]);
		    v.blue() = (unsigned char) TIFFGetB(raster[idx]);
		    ima((*fun)(i, j, ni_1, nj_1)) = v;

		    ++idx;
		  }
	    }
	  else
	    {
	      nj_1 = ima.nrows() - 1;
	      ni_1 = ima.ncols() - 1;
	      for (int j = 0; j <= nj_1; ++j)
		for (int i = 0; i <= ni_1; ++i)
		  {
		    value::rgb8 v;

		    v.red() = (unsigned char) TIFFGetR(raster[idx]);
		    v.green() = (unsigned char) TIFFGetG(raster[idx]);
		    v.blue() = (unsigned char) TIFFGetB(raster[idx]);
		    ima((*fun)(i, j, ni_1, nj_1)) = v;

		    ++idx;
		  }
	    }

	  _TIFFfree(raster);
	}




	template <typename I>
        inline
        void load_data_scalar(I& ima, TIFF *file)
	{
	  uint16 samples_per_pixel;
	  TIFFGetField(file, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	  if (samples_per_pixel != 1)
	  {
	    std::cout << "Trying to load a non grayscale TIFF "
		      << "image into a grayscale Milena image." << std::endl;
	    abort();
	  }

	  uint32 npixels = ima.ncols() * ima.nrows();
	  uint32 *raster = (uint32 *) _TIFFmalloc(npixels * sizeof (uint32));

	  if (!TIFFReadRGBAImage(file, ima.ncols(), ima.nrows(), raster, 0))
	  {
	    std::cout << "Error while reading the image file. Is it corrupted?"
		      << std::endl;
	    abort();
	  }

	  uint16 orientation;
	  TIFFGetField(file, TIFFTAG_ORIENTATION, &orientation);

	  typedef point2d (*fun_t)(int, int, int, int);
	  fun_t funs[] = { 0, ij2rc_1, ij2rc_2, ij2rc_3, ij2rc_4,
			      ij2rc_5, ij2rc_6, ij2rc_7, ij2rc_8 };
	  fun_t fun = funs[orientation];

	  int ni_1, nj_1;
	  unsigned idx = 0;

	  if (orientation <= 4)
	    {
	      ni_1 = ima.nrows() - 1;
	      nj_1 = ima.ncols() - 1;
	      for (int i = 0; i <= ni_1; ++i)
		for (int j = 0; j <= nj_1; ++j)
		  {
		    ima((*fun)(i, j, ni_1, nj_1)) = (unsigned char) TIFFGetR(raster[idx++]);
		  }
	    }
	  else
	    {
	      nj_1 = ima.nrows() - 1;
	      ni_1 = ima.ncols() - 1;
	      for (int j = 0; j <= nj_1; ++j)
		for (int i = 0; i <= ni_1; ++i)
		  {
		    ima((*fun)(i, j, ni_1, nj_1)) = (unsigned char) TIFFGetR(raster[idx++]);
		  }
	    }

	  _TIFFfree(raster);
	}

	template <typename I>
	inline
	void
	load_data_dispatch(const value::rgb8&, I& ima, TIFF *file)
	{
	  load_data_rgb8(ima, file);
	}

	template <typename S, typename I>
	inline
	void
	load_data_dispatch(const value::Scalar<S>&, I& ima, TIFF *file)
	{
	  load_data_scalar(ima, file);
	}

	template <typename I>
	inline
	void
	load_data_dispatch(const bool&, I& ima, TIFF *file)
	{
	  load_data_scalar(ima, file);
	}


	template <typename I>
	inline
	void
	load_data_dispatch(I& ima, TIFF *file)
	{
	  load_data_dispatch(mln_value(I)(), exact(ima), file);
	}


      } // end of namespace mln::io::tiff::internal



      // Facade

      template <typename I>
      inline
      void load(Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::tiff::load");

	I& ima = exact(ima_);

	TIFF *file = TIFFOpen(filename.c_str(), "r");
	if (file == 0)
	{
	  std::cerr << "io::tiff::load - Error: cannot open file '"
		    << filename << "'!"
		    << std::endl;
	  abort();
	}

	ima = internal::load_header<I>(file);
	internal::load_data_dispatch(ima, file);

	mln_postcondition(ima.is_valid());

	(void) TIFFClose(file);
	trace::exiting("mln::io::tiff::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::tiff

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_TIFF_LOAD_HH
