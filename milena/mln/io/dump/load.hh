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

#ifndef MLN_IO_DUMP_LOAD_HH
# define MLN_IO_DUMP_LOAD_HH

/// \file
///
/// Load a Milena image dumped into a file.

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

    namespace dump
    {

      /*! \brief Load a Milena image by dumped into a file.
       *
       * \param[in,out] ima_ The image to load.
       * \param[in] filename the destination.
       *
       * \ingroup iodump
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
	  char tmp[sizeof (P)];
	  file.read(tmp, sizeof (P));
	  p = *(P*)(void*)(&tmp);
	}


	template <typename I>
        inline
        void load_header(Image<I>& ima, std::ifstream& file,
			 const std::string& filename)
	{
	  // Milena's file type ?
	  std::string file_type;
	  file >> file_type;
	  if (file_type != "milena/dump")
	  {
	    std::cerr << "io::dump::load - Error: invalid file type. '"
		      << filename
		      << "' is NOT a valid milena/dump file!"
		      << std::endl;
	    abort();
	  }

	  // Dimension ?
	  unsigned dim;
	  file >> dim;
	  typedef mln_site(I) P;
	  if (P::dim != dim)
	  {
	    std::cerr << "io::dump::load - Error: invalid image dimension. '"
		      << filename << "' is a " << dim << "-D image "
		      << "but you try to load it into a " << P::dim
		      << "-D image!"
		      << std::endl;
	    abort();
	  }

	  // Size information - Skip it, useless.
	  std::string tmp;
	  for (unsigned i = 0; i < dim; ++i)
	    file >> tmp;
	  // Skipping endline.
	  char c;
	  file.get(c);

	  // Value type name ?
	  // WARNING: value type name limited to 255 characters...
	  char value_type[255];
	  file.getline(value_type, 255);
	  if (mln_trait_value_name(mln_value(I)) != std::string(value_type))
	  {
	    std::cerr << "io::dump::load - Error: invalid image value type. '"
		      << filename << "' is an image of '" << value_type
		      << "' but you try to load it into an image of '"
		      << mln_trait_value_name(mln_value(I)) << "'!"
		      << std::endl;
	    abort();
	  }

	  // Empty line - may be used for a new information.
	  file.get(c);

	  // Empty line - may be used for a new information.
	  file.get(c);

	  // Pmin
	  P pmin;
	  read_point<P>(file, pmin);

	  // Pmax
	  P pmax;
	  read_point<P>(file, pmax);

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

      } // end of namespace mln::io::dump::internal



      template <typename I>
      void load(Image<I>& ima, const std::string& filename)
      {
	trace::entering("mln::io::dump::load");

	std::ifstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "io::dump::load - Error: cannot open file '"
		    << filename << "'!"
		    << std::endl;
	  abort();
	}

	internal::load_header(ima, file, filename);
	internal::load_data(ima, file);

	mln_postcondition(exact(ima).is_valid());

	trace::exiting("mln::io::dump::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dump

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_DUMP_LOAD_HH
