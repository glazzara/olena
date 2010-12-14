// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_STATIC_ARRAY_HH
# define MLN_UTIL_STATIC_ARRAY_HH

// FIXME: Add documentation.

# include <cstddef>
# include <algorithm>

# include <mln/core/internal/window_base.hh>
# include <mln/core/concept/gdpoint.hh>

# include <mln/metal/is_a.hh>
# include <mln/util/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/norm/linfty.hh>
# include <mln/literal/zero.hh>

namespace mln
{

  namespace util
  {

    template <typename T, std::size_t n>
    class static_array
    {
    public:
      // FIXME: Careful, this ctor initializes nothing.
      static_array();

      // FIXME: Find a better way to build this static array.
      template <typename InputIterator>
      static_array(InputIterator first, InputIterator last);

      T& operator[](std::size_t i);
      const T& operator[](std::size_t i) const;

      bool has(const T& value) const;

    private:
      T data_[n];
    };


    template <typename D, std::size_t n>
    bool
    operator==(const static_array<D, n>& lhs, const static_array<D, n>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename T, std::size_t n>
    inline
    static_array<T, n>::static_array()
    {
    }

    template <typename T, std::size_t n>
    template <typename InputIterator>
    inline
    static_array<T, n>::static_array(InputIterator first, InputIterator last)
    {
      mln_precondition(std::distance(first, last) == n);
      std::copy(first, last, data_);
    }

    template <typename T, std::size_t n>
    inline
    T&
    static_array<T, n>::operator[](std::size_t i)
    {
      return data_[i];
    }

    template <typename T, std::size_t n>
    inline
    const T& 
    static_array<T, n>::operator[](std::size_t i) const
    {
      return data_[i];
    }

    template <typename T, std::size_t n>
    inline
    bool
    static_array<T, n>::has(const T& value) const
    {
      return std::find(data_, data_ + n, value) != data_ + n;
    }


    template <typename D, std::size_t n>
    inline
    bool
    operator==(const static_array<D, n>& lhs, const static_array<D, n>& rhs)
    {
      for(std::size_t i = 0; i < n; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_STATIC_ARRAY_HH
