// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_METAL_ARRAY_HH
# define MLN_METAL_ARRAY_HH

# include <mln/metal/array1d.hh>
# include <mln/metal/array2d.hh>
# include <mln/metal/array3d.hh>

namespace mln
{

  namespace metal
  {

    // a1
    
    template<unsigned i, class T, unsigned n> inline
    T get_at(const array1d<T, n>& arr)
    {
      return arr.template get_at_<i>();
    }

    template<unsigned i, class T, unsigned n> inline
    T get(const array1d<T, n>& arr)
    {
      return arr.template get_<i>();
    }

    // a2
    
    template<unsigned row, unsigned col, class T,
             unsigned r, unsigned c> inline
    T get_at(const array2d<T, r, c>& arr)
    {
      return arr.template get_at_<row, col>();
    }

    template<unsigned row, unsigned col, class T,
             unsigned r, unsigned c> inline
    T get(const array2d<T, r, c>& arr)
    {
      return arr.template get_<row, col>();
    }

    // a3

    template<unsigned sli, unsigned row, unsigned col,
             class T, unsigned s, unsigned r, unsigned c> inline
    T get_at(const array3d<T, s, r, c>& arr)
    {
      return arr.template get_at_<sli, row, col>();
    }

    template<unsigned sli, unsigned row, unsigned col,
             class T, unsigned s, unsigned r, unsigned c> inline
    T get_(const array3d<T, s, r, c>& arr)
    {
      return arr.template get_<sli, row, col>();
    }
    
    // print

    template<typename T, unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const array1d<T, n>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
        ostr << rhs[i] << " ";
      ostr << std::endl;

      return ostr;
    }

    template<typename T, unsigned r, unsigned c>
    std::ostream& operator<<(std::ostream& ostr, const array2d<T, r, c>& rhs)
    {
      for (unsigned i = 0; i < r; ++i)
        {
          for (unsigned j = 0; j < c; ++j)
            ostr << rhs(i,j) << '\t';
          ostr << '\n';
        }
      ostr << std::endl;

      return ostr;
    }

  }

}

#endif // ! MLN_METAL_ARRAY_HH
