// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_IO_TIFF_LOAD_HH
# define MLN_IO_TIFF_LOAD_HH


/// \file mln/io/tiff/load.hh
///
/// Load TIFF images to Milena images.
///
/// \todo Add support for several tiles.


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


      /// Load a TIFF image to a Milena image.
      //
      template <typename I>
      void load(Image<I>& ima_, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename I>
        inline
        void load_header(Image<I>& ima, TIFF *file)
	{
	  uint32 width, height;

	  TIFFGetField(file, TIFFTAG_IMAGEWIDTH, &width);
	  TIFFGetField(file, TIFFTAG_IMAGELENGTH, &height);

	  mln_concrete(I) new_ima(height, width, 0);
	  exact(ima) = new_ima;

	}


	template <typename I>
        inline
        void load_data_rgb8(I& ima, TIFF *file)
	{
	  uint16 bits_per_sample, samples_per_pixel;
	  TIFFGetField(file, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	  TIFFGetField(file, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	  uint16 data_size = bits_per_sample * samples_per_pixel;
	  if (data_size != 24 || data_size != 32)
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

	  unsigned i = ima.ncols() - 1;
	  unsigned j = 0;
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	  {
	    unsigned idx = i * ima.ncols() + j;
	    value::rgb8 v;
	    v.red() = (unsigned char) TIFFGetR(raster[idx]);
	    v.green() = (unsigned char) TIFFGetG(raster[idx]);
	    v.blue() = (unsigned char) TIFFGetB(raster[idx]);
	    ima(p) = v;
	    ++j;
	    if (!(j%ima.nrows()))
	    {
	      --i;
	      j = 0;
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

	  TIFFReadRGBAImage(file, ima.ncols(), ima.nrows(), raster, 0);

	  unsigned i = ima.ncols() - 1;
	  unsigned j = 0;
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	  {
	    unsigned idx = i * ima.ncols() + j;
	    ima(p) = (unsigned char) TIFFGetR(raster[idx]);
	    ++j;
	    if (!(j%ima.nrows()))
	    {
	      --i;
	      j = 0;
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
	load_data_dispatch(Image<I>& ima, TIFF *file)
	{
	  load_data_dispatch(mln_value(I)(), exact(ima), file);
	}


      } // end of namespace mln::io::tiff::internal



      // Facade

      template <typename I>
      inline
      void load(Image<I>& ima, const std::string& filename)
      {
	trace::entering("mln::io::tiff::load");

	TIFF *file = TIFFOpen(filename.c_str(), "r");
	if (file == 0)
	{
	  std::cerr << "io::tiff::load - Error: cannot open file '"
		    << filename << "'!"
		    << std::endl;
	  abort();
	}

	internal::load_header(ima, file);
	internal::load_data_dispatch(ima, file);

	mln_postcondition(exact(ima).is_valid());

	trace::exiting("mln::io::tiff::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::tiff

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_TIFF_LOAD_HH
