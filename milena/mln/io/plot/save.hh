// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_PLOT_SAVE_HH
# define MLN_IO_PLOT_SAVE_HH

/// \file   mln/io/plot/save.hh
///
/// Define a routine which saves in a plot format.

# include <iostream>
# include <fstream>
# include <mln/core/image/image1d.hh>
# include <mln/util/array.hh>


namespace mln
{

  namespace io
  {

    namespace plot
    {

      /*! Save a Milena 1D image in a plot file.
       *
       * \param[in] ima A reference to the image to save.
       * \param[out] filename The output file.
       * \param[in] start_value The start index value of the plot
       *	    (optional).
       */
      template <typename I>
      void save(image1d<I>& ima,
		const std::string& filename,
		int start_value = 0);

      /*! Save a Milena array in a plot file.
       *
       * \param[in] arr A reference to the array to save.
       * \param[out] filename The output file.
       * \param[in] start_value The start index value of the plot
       *	    (optional).
       */
      template <typename I>
      void save(util::array<I>& arr,
		const std::string& filename,
		int start_value = 0);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      inline
      void save(image1d<I>& ima, const std::string& filename,
		int start_value = 0)
      {
	trace::entering("mln::io::plot::save");

	std::ofstream file_out(filename.c_str());
	for (unsigned i = 0; i < ima.ninds(); ++i)
	  file_out << start_value + i << " " << ima.at_(i) << std::endl;

	trace::exiting("mln::io::plot::save");
      }

      template <typename I>
      inline
      void save(util::array<I>& arr, const std::string& filename,
		int start_value = 0)
      {
	trace::entering("mln::io::plot::save");

	std::ofstream file_out(filename.c_str());
	for (unsigned i = 0; i < arr.nelements(); ++i)
	  file_out << start_value + i << " " << arr[i] << std::endl;

	trace::exiting("mln::io::plot::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::plot

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PLOT_SAVE_HH
