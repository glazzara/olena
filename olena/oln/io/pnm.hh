// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_IO_PNM_HH
# define OLN_IO_PNM_HH

# include <iostream>
# include <fstream>
# include <string>

# include <oln/basics2d.hh>


namespace oln {

  namespace io {

    namespace internal {

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


      /// load_pbm_raw.
      template <typename I>
      void load_pbm_raw(std::ifstream& file, I& ima)
      {
	oln_coord(I) cols = 0;
	unsigned bits = 0;
	unsigned char c = 0;
	oln_fwd_piter(I) p(ima.topo());
	for_all(p)
	  {
	    if (bits == 0)
	      {
		c = file.get();
		bits = 8;
	      }
	    ima(p) = (c & (1 << --bits)) ? false : true;
	    if (++cols >= int(ima.bbox().ncols()))
	      cols = bits = 0;
	  }
      }


      /// load_pbm_ascii.
      template <typename I>
      void load_pbm_ascii(std::ifstream& file, I& ima)
      {
	oln_fwd_piter(I) p(ima.topo());
	for_all(p)
	  ima(p) = (file.get() == '0');
      }


      /// load_ascii.
      template <typename I>
      void load_pnm_ascii(std::ifstream& file, I& ima)
      {
	oln_fwd_piter(I) p(ima.topo());
	for_all(p)
	  {
	    oln_value(I) value;
	    file >> value;
	    ima(p) = value;
	    // FIXME: Test alt code below.
	    // file >> ima(p);
	  }
      }


      /// load_raw_2d.
      template <typename I>
      void load_pnm_raw_2d(std::ifstream& file, I& ima)
      {
	int col = ima.pmin().col();
	size_t len = ima.bbox().ncols();
	for (int row = ima.pmin().row(); row <= ima.pmax().row(); ++row)
	  file.read((char*)(ima.adr_at(row, col)),
		    len * sizeof(oln_value(I)));
      }


    } // end of namespace oln::io::internal


    image2d<bool> load_pbm(const std::string& filename)
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
      internal::read_pnm_header('1', '4', file, type, nrows, ncols);
      image2d<bool> ima(nrows, ncols);
      if (type == '4')
	internal::load_pbm_raw(file, ima);
      else
	// type == '1'
	internal::load_pbm_ascii(file, ima);
      return ima;
    }


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
      if (type == '4')
	internal::load_pnm_raw_2d(file, ima);
      else
	// type == '1'
	internal::load_pnm_ascii(file, ima);
      return ima;
    }

  } // end of namespace oln::io

} // end of namespace oln


#endif // ! OLN_IO_PNM_HH
