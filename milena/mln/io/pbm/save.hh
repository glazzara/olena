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

#ifndef MLN_IO_PBM_SAVE_HH
# define MLN_IO_PBM_SAVE_HH

/*!
 * \file   mln/io/pbm/save.hh
 *
 * \brief Define a function which saves an image of kind pbm into
 * given path.
 *
 */

# include <iostream>
# include <fstream>

# include <mln/geom/size2d.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/bexpr.hh>

# include <mln/io/pnm/save.hh>


namespace mln
{

  // Fwd decl.
  namespace value {
    template <unsigned> class int_u;
    template <unsigned> class int_u_sat;
  }


  namespace io
  {

    namespace pbm
    {

      /*! Save a Milena image as a pbm image.
       *
       * \param[in] ima The image to save.
       * \param[in,out] filename the destination.
       */
      template <typename I>
      void save(const Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <typename I>
	inline
	void save_(const Image<I>& ima_, const std::string& filename)
	{
	  const I& ima = exact(ima_);
	  std::ofstream file(filename.c_str());

	  io::pnm::save_header(PBM, ima, filename, file);

	  int ncols = geom::ncols(ima);
	  int col = 0;
	  int stride = 0;
	  unsigned char c = 0;

	  mln_fwd_piter(I) p(ima.domain());
	  for_all(p)
	    {
	      c <<= 1;
	      c += (ima(p) == true ? 0 : 1); // In pbm, '0' means 'white' so 'object', thus 'true'!
	      if (++col >= ncols)
		{
		  c <<= (8 - stride - 1);
		  file << c;
		  c = col = stride = 0;
		}
	      else
		if (++stride >= 8)
		  {
		    file << c;
		    c = stride = 0;
		  }
	    }
	  mln_postcondition(stride == 0);
	}

      } // end of namespace mln::io::impl


      template <typename I>
      inline
      void save(const Image<I>& ima, const std::string& filename)
      {
	trace::entering("mln::io::pbm::save");
	mln::metal::equal<mln_value(I), bool >::check();
	impl::save_(exact(ima), filename);
	trace::exiting("mln::io::pbm::save");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::pbm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PBM_SAVE_HH
