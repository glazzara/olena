// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_DICOM_LOAD_HH
# define MLN_IO_DICOM_LOAD_HH

/*!
 * \file   mln/io/magick/load.hh
 *
 * \brief Define a function which loads an image of kind magick with
 * given path.
 *
 */

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>

# include <mln/algebra/vec.hh>

# include <gdcmReader.h>
# include <gdcmImageReader.h>
# include <gdcmWriter.h>
# include <gdcmDataSet.h>
# include <gdcmAttribute.h>


namespace mln
{

  namespace io
  {

    namespace dicom
    {

      /*! Load a dicom image in a Milena image.
       *
       * \param[out] ima A reference to the image which will receive
       * data.
       * \param[in] filename The source.
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
	trace::entering("mln::io::gdcm::load");

	using value::int_u16;
	I& ima = exact(ima_);

	gdcm::ImageReader r;
	r.SetFileName(filename.c_str());
	if (!r.Read())
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	gdcm::File &file = r.GetFile();
	gdcm::DataSet& ds = file.GetDataSet();

	gdcm::Image& image = r.GetImage();

	char* dataBuffer = new char[image.GetBufferLength()];
	image.GetBuffer(dataBuffer);

	int ndims = image.GetNumberOfDimensions();
	const unsigned int* dims = image.GetDimensions();

	algebra::vec<mln_site_(I)::dim, unsigned int> vmin;
	algebra::vec<mln_site_(I)::dim, unsigned int> vmax;
	algebra::vec<mln_site_(I)::dim, unsigned int> vdims;
	for (int i = ndims - 1; i >= 0; --i)
	{
	  vmin[i] = 0;
	  vmax[i] = dims[ndims - i - 1] - 1;
	  if (i == ndims - 1)
	    vdims[i] = 1;
	  else
	    vdims[i] = dims[ndims - i - 2] * vdims[i + 1];
	}

	mln_site(I) pmin(vmin);
	mln_site(I) pmax(vmax);
	mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	initialize(ima, result);
	mln_piter(I) p(ima.domain());
	// FIXME: Check ScalarType and HighBit
	unsigned int index = 0;
	for_all(p)
	{
	  index = 0;
	  for (int i = 0; i < ndims; ++i)
	  {
	    index += p.to_site().to_vec()[i] * vdims[i];
	  }
	  ima(p) = dataBuffer[index * 2] * 256 + dataBuffer[index * 2 + 1];
	}

	delete(dataBuffer);

	trace::exiting("mln::io::dicom::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dicom

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_DICOM_LOAD_HH
