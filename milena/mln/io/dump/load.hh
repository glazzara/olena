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

#ifndef MLN_IO_DUMP_LOAD_HH
# define MLN_IO_DUMP_LOAD_HH

/// \file mln/io/dump/load.hh
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

      /// Load a Milena image by dumped into a file.
      ///
      /// \param[in,out] ima_ The image to load.
      /// \param[in] filename the destination.
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
        void load_header(Image<I>& ima, std::ifstream& file)
	{
	  unsigned dim;
	  file >> dim;

	  typedef mln_site(I) P;
	  mln_assertion(P::dim == dim);

	  P pmin, pmax;
	  read_point<P>(file, pmin);
	  read_point<P>(file, pmax);

	  mln_concrete(I) result(box<P>(pmin, pmax));
	  initialize(ima, result);
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
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	internal::load_header(ima, file);
	internal::load_data(ima, file);

	mln_postcondition(exact(ima).is_valid());

	trace::exiting("mln::io::dump::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dump

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_DUMP_LOAD_HH
