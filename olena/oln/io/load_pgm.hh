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

#ifndef OLN_IO_LOAD_PGM_HH
# define OLN_IO_LOAD_PGM_HH

# include <iostream>
# include <fstream>
# include <string>

# include <oln/core/2d/image2d.hh>


namespace oln {

  namespace io {

    namespace internal {

      // Forward declaration
      void abort();

      bool read_pnm_header(std::istream& istr,
			   char& type,
			   int& nrows, int& ncols,
			   bool test = false);

      void read_pnm_header(char ascii, char raw,
			   std::istream& istr,
			   char& type,
			   int& nrows, int& ncols);

      template <typename I>
      void load_pgm_ascii(std::ifstream& file, I& ima);

      template <typename I>
      void load_pgm_raw_2d(std::ifstream& file, I& ima);


#ifndef OLN_INCLUDE_ONLY

      void abort()
      {
	std::cerr << " aborting." << std::endl;
	exit(0);
      }

      bool read_pnm_header(std::istream& istr,
			   char& type,
			   int& nrows, int& ncols,
			   bool test)
      {
	// check magic
	if (istr.get() != 'P' )
	  goto err;
	type = istr.get();
	if (type < '1' or type > '6')
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
	if (nrows <= 0 or ncols <= 0)
	  goto err;

	// skip maxvalue
	if (istr.get() != '\n')
	  goto err;
	if (type != '1' and type != '4')
	{
	  std::string line;
	  std::getline(istr, line);
	}
	return true;

      err:
	if (not test)
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
	if (not (type == ascii or type == raw))
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
	oln_fwd_piter(I) p(ima.points());
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
	int col = ima.min_col();
	size_t len = ima.ncols() * sizeof(oln_value(I));
	for (int row = ima.min_row(); row <= ima.max_row(); ++row)
	  file.read((char*)(&(ima.at(row, col))), len);
      }

#endif // ! OLN_INCLUDE_ONLY


    } // end of namespace oln::io::internal


    image2d<unsigned char> load_pgm(const std::string& filename);

#ifndef OLN_INCLUDE_ONLY

    image2d<unsigned char> load_pgm(const std::string& filename)
    {
      std::ifstream file(filename.c_str());
      if (not file)
	{
	  std::cerr << "error: file '" << filename
		    << "' not found!";
	  abort();
	}
      char type;
      int nrows, ncols;
      internal::read_pnm_header('2', '5', file, type, nrows, ncols);
      image2d<unsigned char> ima(nrows, ncols);
      if (type == '5')
	internal::load_pgm_raw_2d(file, ima);
      else
	if (type == '2')
	  internal::load_pgm_ascii(file, ima);
      return ima;
    }

#endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::io

} // end of namespace oln


#endif // ! OLN_IO_LOAD_PGM_HH
