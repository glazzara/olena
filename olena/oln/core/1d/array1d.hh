// Copyright (C) 2001, 2003, 2004, 2006 EPITA Research and Development
// Laboratory
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

#ifndef OLN_CORE_1D_ARRAY1D_HH
# define OLN_CORE_1D_ARRAY1D_HH

# include <cstdlib>
# include <mlc/contract.hh>


namespace oln
{

  /// General 1D array class.
  template <typename value_t, typename coord_t = int>
  class array1d
  {
  public:

    /// Ctor.
    array1d(coord_t min, coord_t max) :
      min_(min), max_(max)
    {
      precondition(max >= min);
      len_ = max - min + 1;
      allocate_();
    }

    /// Ctor.
    array1d(coord_t len) :
      min_(0), len_(len)
    {
      precondition(len > 0);
      max_ = min_ + len_;
      allocate_();
    }

    /// Dtor.
    ~array1d()
    {
      deallocate_();
    }

    value_t operator()(coord_t i) const
    {
      precondition(has(i));
      return buffer_[i - min_];
    }

    value_t& operator()(coord_t i)
    {
      precondition(has(i));
      return buffer_[i - min_];
    }

    bool has(coord_t i) const
    {
      return
	i >= min_ and i <= max_;
    }

    size_t memsize() const
    {
      return size_t(len_) * sizeof(value_t);
    }

  protected:

    coord_t   min_, max_;
    coord_t   len_;
    value_t*  buffer_;

  private:

    void allocate_()
    {
      buffer_ = new value_t[size_t(len_)];
    }

    void deallocate_()
    {
      precondition(buffer_ != 0);
      delete[] buffer_;
      buffer_ = 0; // safety
    }
  };


} // end of namespace oln


#endif // ! OLN_CORE_1D_ARRAY1D_HH
