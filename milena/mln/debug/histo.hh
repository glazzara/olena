// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DEBUG_HISTO_HH
# define MLN_DEBUG_HISTO_HH

/// \file mln/debug/histo.hh
///
/// Save a histogram to a plot data file.
///
/// \todo Move to io::histo::save.

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <mln/histo/array.hh>
# include <mln/core/image/image2d.hh>
# include <mln/draw/line.hh>


namespace mln
{

  namespace debug
  {


    template <typename T>
    void
    histo(const histo::array<T>& h, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    void
    histo(const histo::array<T>& h, const std::string& filename)
    {
      std::ofstream file(filename.c_str());
      if (! file)
      {
	std::cerr << "error: cannot open file '" << filename << "'!";
	std::abort();
      }
      mln_viter(value::set<T>) v(h.vset());
      for_all(v)
	file << v << ' ' << h(v) << std::endl;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_HISTO_HH
