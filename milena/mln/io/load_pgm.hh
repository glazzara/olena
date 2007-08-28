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

#ifndef MLN_IO_LOAD_PGM_HH
# define MLN_IO_LOAD_PGM_HH

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image2d_b.hh>
# include <mln/value/int_u8.hh>


namespace mln
{

  namespace io
  {

    namespace internal
    {

      void abort()
      {
	std::cerr << " aborting." << std::endl;
	exit(0);
      }

      bool read_pnm_header(std::istream& istr,
			   char& type,
			   int& nrows, int& ncols,
			   bool test = false)
      {
	// check magic
	if (istr.get() != 'P' )
	  goto err;
	type = istr.get();
	if (type < '1' || type > '6')
	  goto err;
	if (istr.get() != '\n')
	  goto err;

	// skip comments
	while (istr.peek() == '#')
	{
	  std::string line;
	  std::getline(istr, line);
	}

	// get size
	istr >> ncols >> nrows;
	if (nrows <= 0 || ncols <= 0)
	  goto err;

	// skip maxvalue
	if (istr.get() != '\n')
	  goto err;
	if (type != '1' && type != '4')
	{
	  std::string line;
	  std::getline(istr, line);
	}
	return true;

      err:
	if (! test)
	  {
	    std::cerr << "error: badly formed header!";
	    abort();
	  }
	return false;
      }

      void read_pnm_header(char ascii, char raw,
			   std::istream& istr,
			   char& type,
			   int& nrows, int& ncols)
      {
	read_pnm_header(istr, type, nrows, ncols);
	if (! (type == ascii || type == raw))
	  {
	    std::cerr << "error: bad pnm type; "
		      << "expected P" << ascii
		      << " or P" << raw
		      << ", get P" << type << "!";
	    abort();
	  }
      }


      /// load_ascii.
      template <typename I>
      void load_pgm_ascii(std::ifstream& file, I& ima)
      {
	mln_fwd_piter(I) p(ima.domain());
	for_all(p)
	  {
	    unsigned value;
	    file >> value;
	    ima(p) = value;
	    // FIXME: Test alt code below.
	    // file >> ima(p);
	  }
      }


      /// load_raw_2d.
      template <typename I>
      void load_pgm_raw_2d(std::ifstream& file, I& ima)
      {
	point2d p = make::point2d(0, ima.domain().pmin().col());
	typedef mln_value(I) V;
	const mln_coord(I)
	  min_row = geom::min_row(ima),
	  max_row = geom::max_row(ima);
	if (sizeof(V) == 1)
	  {
	    size_t len = geom::ncols(ima) * sizeof(mln_enc(V));
	    for (p.row() = min_row; p.row() <= max_row; ++p.row())
	      file.read((char*)(& ima(p)), len);
	  }
	else
	  {
	    // FIXME: code for g++-2.95 when sizeof(int_u8) == 2!!!
	    const mln_coord(I)
	      min_col = geom::min_col(ima),
	      max_col = geom::max_col(ima);
	    for (p.row()  = min_row; p.row() <= max_row; ++p.row())
	      for (p.col()  = min_col; p.col() <= max_col; ++p.col())
		{
		  unsigned char c;
		  file.read((char*)(&c), 1);
		  ima(p) = c;
		}
	  }
      }


    } // end of namespace mln::io::internal


    image2d_b<value::int_u8> load_pgm(const std::string& filename)
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
      internal::read_pnm_header('2', '5', file, type, nrows, ncols);

      image2d_b<value::int_u8> ima(nrows, ncols);
      if (type == '5')
	internal::load_pgm_raw_2d(file, ima);
      else
	if (type == '2')
	  internal::load_pgm_ascii(file, ima);
      return ima;
    }

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_LOAD_PGM_HH
