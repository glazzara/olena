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

#ifndef MLN_IO_DUMP_SAVE_HH
# define MLN_IO_DUMP_SAVE_HH

/// \file
///
/// Save a Milena image by dumping its data to a file.
///
/// \todo handle endianness.

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/core/pixel.hh>
# include <mln/data/memcpy_.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace io
  {

    namespace dump
    {

      /*! Save a Milena image by dumping its data to a file.
       *
       * \param[in] ima_ The image to save.
       * \param[in] filename the destination.
       *
       * \ingroup iodump
       */
      template <typename I>
      void save(const Image<I>& ima_, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I>
        inline
        void save_header(const I& ima,
			 std::ofstream& file)
	{
	  // Milena's file type
	  file << "milena/dump" << std::endl;

	  // Dimension
	  typedef mln_site(I) P;
	  file << P::dim << std::endl;

	  // Image size.
	  typedef algebra::vec<P::dim, unsigned> vec_t;
	  vec_t size = ima.domain().pmax() - ima.domain().pmin();
	  for (unsigned i = 0; i < P::dim - 1; ++i)
	    file << size[i] + 1 << " ";
	  file << size[P::dim - 1] + 1 << std::endl;

	  // Value type name
	  // WARNING: value type name limited to 255 characters...
	  file << mln_trait_value_name(mln_value(I)) << std::endl;

	  // Empty line - may be used for a new information.
	  file << std::endl;

	  // Empty line - may be used for a new information.
	  file << std::endl;

	  // Pmin
	  mln_site(I) p = ima.domain().pmin();
	  file.write((char*) (&p), sizeof (P));

	  // Pmax
	  p = ima.domain().pmax();
	  file.write((char*) (&p), sizeof (P));
	}


	template <typename I>
        inline
        void save_data(I& ima, std::ofstream& file)
	{
	  // Handle padding.
	  unsigned data_size = sizeof (mln_value(I)) + sizeof (mln_value(I)) % 2;

	  mln_box_runstart_piter(I) p(ima.domain());
	  for_all(p)
	  {
	    pixel<I> src(ima, p);
	    file.write((char*) (&src.val()), p.run_length() * data_size);
	  }
	}

      } // end of namespace mln::io::dump::internal



      // Facade

      template <typename I>
      void save(const Image<I>& ima_, const std::string& filename)
      {
	mln_trace("mln::io::dump::save");

	const I& ima = exact(ima_);

	std::ofstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	internal::save_header(ima, file);
	internal::save_data(ima, file);


	file.close();

      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dump

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_DUMP_SAVE_HH
