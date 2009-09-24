// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PNM_LOAD_DISK_IMA_HH
# define MLN_IO_PNM_LOAD_DISK_IMA_HH

/// \file
///
/// Define a function which loads an image of kind PNM 8/16bits with
/// given path.

# include <iostream>
# include <fstream>
# include <string>

# include <mln/io/pnm/load_header.hh>
# include <mln/io/pnm/max_component.hh>
# include <mln/io/pnm/macros.hh>

# include <mln/metal/is_a.hh>

namespace mln
{

  namespace io
  {

    namespace pnm
    {

# ifndef MLN_INCLUDE_ONLY


      // Warning if we try to load uncontiguous 2D PNM file into a disk
      // image, this is not currently supported.
      template <typename V>
      inline
      void load_raw_2d_uncontiguous(std::ifstream& file, disk_ima<V>& ima)
      {
	(void) file;
	(void) ima;
	std::cout << "You can't load an uncontiguous 2D PNM file into ";
	std::cout << "a disk image" << std::endl;
	abort();
      }

      // Load raw 2d for disk images.
      template <typename T>
      inline
      void load_raw_2d_contiguous(std::ifstream& file, disk_ima<T>& ima, const std::string& filename)
      {
	ima.pos_() = file.tellg();
	ima.file_() = filename;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pnm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_LOAD_DISK_IMA_HH
