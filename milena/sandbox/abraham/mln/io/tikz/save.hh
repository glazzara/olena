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

#ifndef MLN_IO_TIKZ_SAVE_HH
# define MLN_IO_TIKZ_SAVE_HH

/*!
 * \file   mln/io/tikz/save.hh
 *
 * \brief Define a function which saves an image of kind TIKZ into
 * given path.
 *
 */

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/point2d.hh>

# include <mln/value/graylevel.hh>
# include <mln/value/rgb.hh>
# include <mln/value/int_u.hh>

# include <mln/metal/templated_by.hh>

# include <mln/io/tikz/save_header.hh>

# include <mln/geom/size2d.hh>

namespace mln
{

  namespace io
  {

    namespace tikz
    {

      /*! Save a milena image as a tikz image.
       *
       * \param[in] ima_ The image to save.
       * \param[in,out] filename the destination.
       */
      template <typename I>
      void save(const Image<I>& ima_, const std::string& filename);

      /*! Save a milena image as a tikz image with values.
       *
       * \param[in] ima_ The image to save.
       * \param[in,out] filename the destination.
       * \param[in] val_ An Image containing values to print.
       */
      template <typename I, typename V>
      void save(const Image<I>& ima_, const std::string& filename, const Image<V>& val_);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// write a greylevel value into
	template <unsigned int n>
	inline
	void write_value(std::ofstream& file,
			 const value::int_u<n>& v)
	{
	  int c = v;
	  file << "black!" << (int(mln::trait::value_< mln::value::int_u<n> >::max()) - c) * 100 / int(mln::trait::value_< mln::value::int_u<n> >::max());
	}

	// write a rgb value into
	template <unsigned int n>
	inline
	void write_value(std::ofstream& file,
			 const value::rgb<n>& v)
	{
	  mln::value::int_u<n> cvalues[] = {v.red(), v.green(), v.blue()};
	  char *cnames[] = {"red", "green", "blue"};
	  short colors[] = {0, 1, 2}, tmp;

	  // Sort colors increasing
	  if (cvalues[colors[0]] > cvalues[colors[1]])
	    {
	      tmp = colors[0];
	      colors[0] = colors[1];
	      colors[1] = tmp;
	    }

	  if (cvalues[colors[1]] > cvalues[colors[2]])
	    {
	      tmp = colors[1];
	      colors[1] = colors[2];
	      colors[2] = tmp;
	    }

	  if (cvalues[colors[0]] > cvalues[colors[1]])
	    {
	      tmp = colors[0];
	      colors[0] = colors[1];
	      colors[1] = tmp;
	    }

	  double c1 = cvalues[colors[1]] - cvalues[colors[0]],
	    c2 = cvalues[colors[2]] - cvalues[colors[0]];

	  double w = 1 - double(cvalues[colors[0]]) / 255.;

	  c1 /= w;
	  c2 /= w;

	  double c = 1 - c1 / 255.;

	  c2 -= c1;
	  c2 /= c;

	  double p = 1 - c2 / 255.;

	  file << "black!" << int(p * 100) << "!" << cnames[colors[2]] << "!"
	       << int(c * 100) << "!" << "-" << cnames[colors[0]] << "!"
	       << int(w * 100) << "!white";
	}

	// Save data
	template <typename I>
	inline
	void save_data_(std::ofstream& file,
			const I& ima)
	{
	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	      {
		file << "\\path (" << p.col() << "," << max_row - p.row() << ") node[fill=";
		write_value(file, ima(p));
		file << "] (p_" << p.row() << "_" << p.col() << ") { \\color{";
		file << "black";
		file << "}" << "};" << std::endl;
	      }
	}

	// Save data
	template <typename I, typename V>
	inline
	void save_data_with_values_(std::ofstream& file,
				    const I& ima,
				    const V& val)
	{
	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	      {
		file << "\\path (" << p.col() << "," << max_row - p.row() << ") node[fill=";
		write_value(file, ima(p));
		file << "] (p_" << p.row() << "_" << p.col() << ") { \\color{";
		file << "black";
		file << "}" << val(p) << "};" << std::endl;
	      }
	}


      } // end of namespace mln::io::pnm::impl


	// Facades.

      template <typename I>
      inline
      void save(const Image<I>& ima_, const std::string& filename)
      {
	const I& ima = exact(ima_);
	std::ofstream file(filename.c_str());

	io::tikz::save_header(ima, filename, file);

	impl::save_data_(file,
			 ima);

	file << "\\end{tikzpicture}" << std::endl;
	file << "\\end{document}" << std::endl;
      }

      template <typename I, typename V>
      inline
      void save(const Image<I>& ima_, const std::string& filename, const Image<V>& val_)
      {
	const I& ima = exact(ima_);
	const V& val = exact(val_);
	std::ofstream file(filename.c_str());

	io::tikz::save_header(ima, filename, file);

	impl::save_data_with_values_(file,
				     ima,
				     val);

	file << "\\end{tikzpicture}" << std::endl;
	file << "\\end{document}" << std::endl;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::tikz

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_TIKZ_SAVE_HH
