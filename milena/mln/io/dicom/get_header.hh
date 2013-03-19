// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_DICOM_GET_HEADER_HH
# define MLN_IO_DICOM_GET_HEADER_HH

/// \file
///
/// Load a DICOM file header.

# include <iostream>
# include <fstream>

# include <gdcm-2.0/gdcmReader.h>
# include <gdcm-2.0/gdcmImageReader.h>
# include <gdcm-2.0/gdcmWriter.h>
# include <gdcm-2.0/gdcmDataSet.h>
# include <gdcm-2.0/gdcmAttribute.h>

# include <mln/core/concept/image.hh>
# include <mln/core/routine/initialize.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/core/pixel.hh>
# include <mln/data/memcpy_.hh>
# include <mln/util/array.hh>

namespace mln
{

  namespace io
  {

    namespace dicom
    {

      /*! \brief Store dicom file header.
       *
       * \ingroup iodicom
       */
      struct dicom_header
      {
	// The number of dimensions.
	unsigned dim;

	// The size in each dimension.
	util::array<unsigned> size;
      };


      /*! \brief Retrieve header from a dicom file.
       *
       * \ingroup iodicom
       */
      dicom_header get_header(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


      dicom_header get_header(const std::string& filename)
      {
	mln_trace("mln::io::dicom::get_header");

	dicom_header header;

	gdcm::ImageReader r;
	r.SetFileName(filename.c_str());
	if (!r.Read())
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	gdcm::Image& image = r.GetImage();

	header.dim = image.GetNumberOfDimensions();
	const unsigned int* dims = image.GetDimensions();

	for (unsigned i = 2; i < header.dim; ++i)
	  header.size.append(dims[i]); // sli, ...
	for (unsigned i = 0; i < 2; ++i)
	  header.size.append(dims[i]); // row, col

	return header;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dicom

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_DICOM_GET_HEADER_HH

