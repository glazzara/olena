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

#ifndef MLN_IO_PPM_LOAD_HH
# define MLN_IO_PPM_LOAD_HH

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image2d.hh>
# include <mln/value/rgb8.hh>

# include <mln/io/internal/pnm/load.hh>

namespace mln
{

  namespace io
  {

    namespace ppm
    {
      template <typename V>
      image2d<V> load(const std::string& filename)
      {
	return io::internal::pnm::load<V>(PPM, filename);
      }

      image2d<value::rgb8> load(const std::string& filename)
      {
	return load< value::rgb8 >(filename);
      }

      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename)
      {
	io::internal::pnm::load<I>(PPM, ima, filename);
      }

    } // end of namespace mln::io::ppm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PPM_LOAD_HH

