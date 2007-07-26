// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_METAL_VEC_HH
# define MLN_CORE_METAL_VEC_HH

# include <cstdarg>

# include <mln/core/concept/object.hh>


namespace mln
{

  namespace metal
  {

    // FIXME: Doc! + Change coord into comp.

    template <unsigned n, typename T>
    struct vec : public Object< vec<n,T> >
    {
      enum { dim = n };
      typedef T coord;

      T& operator[](unsigned i);
      T  operator[](unsigned i) const;
    
    protected:
      T coord_[n];
    };


    template <unsigned n, typename T>
    std::ostream& operator<<(std::ostream& ostr, const vec<n,T>& v);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename T>
    T&
    vec<n,T>::operator[](unsigned i)
    {
      mln_precondition(i < n);
      return coord_[i];
    }

    template <unsigned n, typename T>
    T
    vec<n,T>::operator[](unsigned i) const
    {
      mln_precondition(i < n);
      return coord_[i];
    }
    
    template <unsigned n, typename T>
    std::ostream& operator<<(std::ostream& ostr, const vec<n,T>& v)
    {
      ostr << "[ ";
      for (unsigned i = 0; i < n; ++i)
	ostr << v[i] << ' ';
      return ostr << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  
  } // end of namespace mln::metal
  
} // end of namespace mln


#endif // ! MLN_CORE_METAL_VEC_HH
