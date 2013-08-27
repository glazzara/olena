// Copyright (C) 2010, 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_RAW_LOAD_HH
# define MLN_IO_RAW_LOAD_HH

/// \file
///
/// Load an image saved as a raw data file.

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/routine/initialize.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/core/pixel.hh>
# include <mln/data/memcpy_.hh>

namespace mln
{

  namespace io
  {

    namespace raw
    {

      /*! \brief Load an image saved as a raw data file.
       *
       * \param[in,out] ima_ The image to load.
       * \param[in] filename the destination.
       *
       * This routine try to read two input files: 'filename' and
       * 'filename.info'.
       * 'filename' is the raw data.
       * 'filename.info' store various information about the image.
       *
       * \ingroup ioraw
       */
      template <typename I>
      void load(Image<I>& ima_, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename P>
	inline
	void read_point(std::ifstream& file, P& p)
	{
	  for (unsigned i = 0; i < P::dim; ++i)
	    file >> p[i];
	}


	template <typename I>
        inline
        void load_header(Image<I>& ima, std::ifstream& info_file,
			 const std::string& filename)
	{
	  // Milena's file type ?
	  std::string file_type;
	  info_file >> file_type;
	  if (file_type != "milena/raw")
	  {
	    std::cerr << "io::raw::load - Error: invalid file type. '"
		      << filename
		      << "' is NOT a valid milena/raw info file!"
		      << std::endl;
	    abort();
	  }

	  char dev_null[255];

	  // Dimension ?
	  // Reading line title "Dim: "
	  info_file.read(dev_null, 5);

	  unsigned dim;
	  info_file >> dim;

	  typedef mln_site(I) P;
	  if (P::dim != dim)
	  {
	    std::cerr << "io::raw::load - Error: invalid image dimension. '"
		      << filename << "' is a " << dim << "-D image "
		      << "but you try to load it into a " << P::dim
		      << "-D image!"
		      << std::endl;
	    abort();
	  }

	  // Size information - Skip it, useless.
	  std::string tmp;
	  for (unsigned i = 0; i < dim; ++i)
	    info_file >> tmp;
	  // Skipping endline.
	  char c;
	  info_file.get(c);

	  // Value type name ?
	  // Reading line title "data type: "
	  info_file.read(dev_null, 11);
	  // WARNING: value type name limited to 255 characters...
	  char value_type[255];
	  info_file.getline(value_type, 255);
	  if (mln_trait_value_name(mln_value(I)) != std::string(value_type))
	  {
	    std::cerr << "io::raw::load - Error: invalid image value type. '"
		      << filename << "' is an image of '" << value_type
		      << "' but you try to load it into an image of '"
		      << mln_trait_value_name(mln_value(I)) << "'!"
		      << std::endl;
	    abort();
	  }

	  // Pmin
	  // Reading line title "top left: "
	  info_file.read(dev_null, 10);
	  P pmin;
	  read_point<P>(info_file, pmin);

	  // Pmax
	  // Reading line title "bottom right: "
	  info_file.read(dev_null, 14);
	  P pmax;
	  read_point<P>(info_file, pmax);

	  // Initialize the image buffer.
	  mln_concrete(I) result(box<P>(pmin, pmax));
	  initialize(ima, result);
	  mln_assertion(exact(ima).is_valid());
	}


	template <typename I>
        inline
        void load_data(Image<I>& ima_, std::ifstream& file)
	{
	  I& ima = exact(ima_);

	  // Handle padding.
	  unsigned data_size = sizeof (mln_value(I)) + sizeof (mln_value(I)) % 2;

	  mln_box_runstart_piter(I) p(ima.domain());
	  for_all(p)
	  {
	    pixel<I> src(ima, p);
	    file.read((char*) (&src.val()), p.run_length() * data_size);
	  }

	}

      } // end of namespace mln::io::raw::internal



      template <typename I>
      void load(Image<I>& ima, const std::string& filename)
      {
	mln_trace("mln::io::raw::load");

	std::ifstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "io::raw::load - error: cannot open file '"
		    << filename << "'!";
	  abort();
	}

	std::string info_filename = filename + ".info";
	std::ifstream info_file(info_filename.c_str());
	if (! info_file)
	{
	  std::cerr << "io::raw::load - error: cannot open file '"
		    << info_filename << "'!";
	  abort();
	}


	internal::load_header(ima, info_file, info_filename);
	internal::load_data(ima, file);

	mln_postcondition(exact(ima).is_valid());

	file.close();
	info_file.close();

      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::raw

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_RAW_LOAD_HH
