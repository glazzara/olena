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

# include <mln/value/int_u16.hh>

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

      /*! Load a gdcm image in a Milena image.
       *
       * \param[out] ima A reference to the image which will receive
       * data.
       * \param[in] filename The source.
       */
      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename);

      /*! Load a gdcm image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image2d<V> load(const std::string& filename);

      /*! Load a gdcm image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image3d<V> load(const std::string& filename);

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

	std::cout << std::endl << "Image information" << std::endl
		  << "=================" << std::endl;
	image.Print(std::cout);

	std::cout << std::endl << "Buffer information" << std::endl
		  << "=================" << std::endl;
	std::cout << "Buffer length: " << image.GetBufferLength() << std::endl;
	char* dataBuffer = new char[image.GetBufferLength()];
	image.GetBuffer(dataBuffer);

	int ndims = image.GetNumberOfDimensions();
	const unsigned int* dims = image.GetDimensions();

	// FIXME: Check ScalarType
	if (ndims == 1)
	{

	}
	if (ndims == 2)
	{
	  //image2d<int_u16> mln_ima(dims[1], dims[0]);
	  //mln_piter(image2d<int_u16>) p(ima.domain());
	  //for_all(p)
	  //{
	  //  ima(p) = dataBuffer[(p.col() + p.row() * dims[0]) * 2] * 256 +
	//	     dataBuffer[(p.col() + p.row() * dims[0]) * 2 + 1];
	  //}
	}
	if (ndims == 3)
	{
	  //image3d<int_u16> ima(dims[2], dims[1], dims[0]);
	  mln_site(I) pmin(0, 0, 0);
	  mln_site(I) pmax(dims[2] - 1, dims[1] - 1, dims[0] - 1);
	  mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	  initialize(ima, result);
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	  {
	    ima(p) = dataBuffer[(p.col() + p.row() * dims[0] + p.sli() * dims[0] * dims[1]) * 2] * 256 +
		     dataBuffer[(p.col() + p.row() * dims[0] + p.sli() * dims[0] * dims[1]) * 2 + 1];
	    std::cout << (p.col() + p.row() * dims[0] + p.sli() * dims[0] * dims[1]) * 2 << " ["
		      << "p.col = " << p.col() << "] ["
		      << "p.row = " << p.row() << "] ["
		      << "p.sli = " << p.sli() << "]"
		      << std::endl;
	  }
	}

	delete(dataBuffer);

	trace::exiting("mln::io::dicom::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dicom

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_DICOM_LOAD_HH
