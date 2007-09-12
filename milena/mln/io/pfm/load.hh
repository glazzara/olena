// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef MLN_IO_PFM_LOAD_HH
# define MLN_IO_PFM_LOAD_HH

// # include <iostream>
// # include <fstream>
// # include <string>

# include <mln/core/image2d_b.hh>
# include <mln/value/int_u8.hh>


namespace mln
{

  namespace io
  {

    namespace pfm
    {

      namespace internal
      {

	void abort()
	{
	  std::cerr << " aborting." << std::endl;
	  exit(0);
	}

	bool read_pfm_header(std::istream& file,
			     int& nrows, int& ncols,
			     bool test = false)
	{
	  std::string tag;

	  // get size
	  file >> nrows;
	  file >> ncols;

	  if (file.get() != '\n')
	    goto err;

	  // check tag == float
	  file >> tag;
	  if (tag != "float")
	    goto err;

	  if (file.get() != '\n')
	    goto err;

	  return true;

	err:
	  if (! test)
	    {
	      std::cerr << "error: badly formed header!"
			<< std::endl;
	      abort();
	    }
	  return false;
	}


	/// load_raw_2d.
	template <typename I>
	void load_raw_2d(std::ifstream& file, I& ima)
	{
	  point2d p = make::point2d(0, 0);

	  const mln_coord(I)
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);

	  unsigned int
	    ncols = geom::ncols(ima);

	  for (p.row() = min_row; p.row() < max_row; ++p.row())
	    file.read((char*)(&(ima(p))),
		      sizeof(float) * ncols);

	}


      } // end of namespace mln::io::internal

      image2d_b<float> load(const std::string& filename)
      {
	std::ifstream file(filename.c_str());
	if (! file)
	  {
	    std::cerr << "error: file '" << filename
		      << "' not found!";
	    abort();
	  }
	char type;
	int nrows, ncols;
	internal::read_pfm_header(file, nrows, ncols);

	image2d_b<float> ima(nrows, ncols);
	internal::load_raw_2d(file, ima);
	return ima;
      }

    } // end of namespace mln::io::pfm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PFM_LOAD_HH
