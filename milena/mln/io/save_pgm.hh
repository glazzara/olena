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

#ifndef MLN_IO_SAVE_PGM_HH
# define MLN_IO_SAVE_PGM_HH

# include <iostream>
# include <fstream>

# include <mln/geom/size2d.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/bexpr.hh>


namespace mln
{

  // Fwd decl.
  namespace value {
    template <unsigned> class int_u;
    template <unsigned> class int_u_sat;
  }


  namespace io
  {

    template <typename I>
    void save_pgm(const Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void save_pgm_header_(const I& ima, const std::string& filename,
			    std::ofstream& file)
      {
	if (! file)
	  {
	    std::cerr << "error: cannot open file '" << filename
		      << "'!";
	    abort();
	  }
	file << "P5" << std::endl;
	file << "# milena" << std::endl;
	file << geom::ncols(ima) << ' ' << geom::nrows(ima) << std::endl;
	file << "255" << std::endl;
      }

      template <typename I>
      void save_pgm_(const Fast_Image<I>& ima_, const std::string& filename)
      {
	const I& ima = exact(ima_);
	std::ofstream file(filename.c_str());
	save_pgm_header_(ima, filename, file);
	const int
	  min_row = geom::min_row(ima),
	  max_row = geom::max_row(ima);
	point2d p;
	if (sizeof(mln_value(I)) == 1)
	  {
	    p.col() = geom::min_col(ima);
	    size_t len = geom::ncols(ima);
	    for (p.row() = min_row; p.row() <= max_row; ++p.row())
	      file.write((char*)(& ima(p)), len);
	  }
	else
	  {
	    // FIXME: code for g++-2.95 when sizeof(int_u8) == 2!!!
	    const int
	      min_col = geom::min_col(ima),
	      max_col = geom::max_col(ima);
	    for (p.row() = min_row; p.row() <= max_row; ++p.row())
	      for (p.col() = min_col; p.col() <= max_col; ++p.col())
		{
		  unsigned char c = ima(p);
		  file.write((char*)(&c), 1);
		}
	  }
      }

      template <typename I>
      void save_pgm_(const Image<I>& ima_, const std::string& filename)
      {
	const I& ima = exact(ima_);
	std::ofstream file(filename.c_str());
	save_pgm_header_(ima, filename, file);
	const int
	  min_row = geom::min_row(ima),
	  max_row = geom::max_row(ima),
	  min_col = geom::min_col(ima),
	  max_col = geom::max_col(ima);
	point2d p;
	for (p.row() = min_row; p.row() <= max_row; ++p.row())
	  for (p.col() = min_col; p.col() <= max_col; ++p.col())
	    {
	      unsigned char c = ima(p);
	      file.write((char*)(&c), 1);
	    }
      }

    } // end of namespace mln::io::impl


    template <typename I>
    void save_pgm(const Image<I>& ima, const std::string& filename)
    {
      mln::metal::or_<
	mln::metal::equal<mln_value(I), value::int_u<8> >,
	mln::metal::equal<mln_value(I), value::int_u_sat<8> >
	>::check();
      impl::save_pgm_(exact(ima), filename);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_SAVE_PGM_HH
