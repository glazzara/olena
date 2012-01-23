// Copyright (C) 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_IO_DICOM_LOAD_HH
# define MLN_IO_DICOM_LOAD_HH

/// \file
/// \brief Define a function which loads a GDCM image.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>

# include <mln/algebra/vec.hh>

# include <gdcm-2.0/gdcmReader.h>
# include <gdcm-2.0/gdcmImageReader.h>
# include <gdcm-2.0/gdcmWriter.h>
# include <gdcm-2.0/gdcmDataSet.h>
# include <gdcm-2.0/gdcmAttribute.h>


namespace mln
{

  namespace io
  {

    namespace dicom
    {

      /*! \brief Load a DICOM file in a Milena image.

          \param[out] ima A reference to the image which will receive
          data.
          \param[in] filename The source.


	  Common compilation flags to link to gdcm if this file is used:

	  -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8

	  \ingroup iodicom
       */
      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      image2d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::gdcm::load");
	image2d<V> ima;// = io::pnm::load<V>(MAGICK, filename);
	trace::exiting("mln::io::gdcm::load");
	return ima;
      }

      template <typename V>
      inline
      image3d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::gdcm::load");
	image2d<V> ima;// = io::pnm::load<V>(MAGICK, filename);
	trace::exiting("mln::io::gdcm::load");
	return ima;
      }


      template <typename I>
      inline
      void load(Image<I>& ima_,
		const std::string& filename)
      {
	trace::entering("mln::io::dicom::load");

	I& ima = exact(ima_);

	gdcm::ImageReader r;
	r.SetFileName(filename.c_str());
	if (!r.Read())
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	//gdcm::File &file = r.GetFile();
	//gdcm::DataSet& ds = file.GetDataSet();

	gdcm::Image& image = r.GetImage();

	char* dataBuffer = new char[image.GetBufferLength()];
	image.GetBuffer(dataBuffer);

	int ndims = image.GetNumberOfDimensions();
	const unsigned int* dims = image.GetDimensions();

	unsigned short bits_allocated = image.GetPixelFormat().GetBitsAllocated();
	unsigned short bytes_allocated = bits_allocated / 8;
	unsigned short bits_stored = image.GetPixelFormat().GetBitsStored();
	unsigned short samples_per_pixel = image.GetPixelFormat().GetSamplesPerPixel();

	unsigned int offset = 8 - (bits_allocated - bits_stored);
	unsigned int off_pow = 1;
	for (unsigned int i = 0; i < offset; ++i)
	{
	  off_pow *= 2;
	}

	if (mln_site_(I)::dim != ndims)
	{
	  std::cerr << "error: dimension mismatch" << std::endl;
	  abort();
	}

	algebra::vec<mln_site_(I)::dim, unsigned int> vmin;
	algebra::vec<mln_site_(I)::dim, unsigned int> vmax;
	algebra::vec<mln_site_(I)::dim, unsigned int> vdims;
	int j = ndims - 1;
	for (int i = 0; i < ndims; ++i, --j)
	{
	  vmin[i] = 0;
	  vmax[i] = dims[i] - 1;
	  if (i == 0)
	    vdims[j] = 1;
	  else
	    vdims[j] = dims[i - 1] * vdims[j + 1];
	}

	if (ndims > 1)
	{
	  std::swap(vmin[0], vmin[1]);
	  std::swap(vmax[0], vmax[1]);
	}

	mln_site(I) pmin(vmin);
	mln_site(I) pmax(vmax);
	mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	initialize(ima, result);
	mln_piter(I) p(ima.domain());
	unsigned int index = 0;

	for_all(p)
	{
	  index = 0;
	  for (int i = 0; i < ndims; ++i)
	  {
	    index += p[i] * vdims[i];
	  }

	  mln_value(I) v = (unsigned char) dataBuffer[(index * bytes_allocated) * samples_per_pixel];
	  // FIXME: RGB support, HighBit if HB == 1
	  for (unsigned int j = 0; j < bytes_allocated; ++j)
	  {
	    v += ((unsigned char) dataBuffer[(index * bytes_allocated + j) * samples_per_pixel]) * 256 * j;
	  }

	  ima(p) = v;
	}

	delete[] dataBuffer;

	trace::exiting("mln::io::dicom::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dicom

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_DICOM_LOAD_HH
