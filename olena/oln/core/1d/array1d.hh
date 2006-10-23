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
# include <oln/core/abstract/array.hh>


namespace oln
{

  /// General 1D array class.
  template <typename value_t, typename coord_t = int>
  class array1d : public abstract::array
  {
  public:

    /// Ctor.
    array1d(coord_t min, coord_t max);
    /// Ctor.
    array1d(coord_t len);

    /// Dtor.
    ~array1d();

    value_t  operator()(coord_t i) const;
    value_t& operator()(coord_t i);

    bool has(coord_t i) const;

    size_t memsize() const;

  protected:

    coord_t   min_, max_;
    coord_t   len_;
    value_t*  buffer_;

  private:

    void allocate_();
    void deallocate_();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename value_t, typename coord_t>
  array1d<value_t, coord_t>::array1d(coord_t min, coord_t max) :
    min_(min), max_(max)
  {
    precondition(max >= min);
    len_ = max - min + 1;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array1d<value_t, coord_t>::array1d(coord_t len) :
    min_(0), len_(len)
  {
    precondition(len > 0);
    max_ = min_ + len_;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array1d<value_t, coord_t>::~array1d()
  {
    deallocate_();
  }

  template <typename value_t, typename coord_t>
  value_t array1d<value_t, coord_t>::operator()(coord_t i) const
  {
    precondition(has(i));
    return buffer_[i - min_];
  }

  template <typename value_t, typename coord_t>
  value_t& array1d<value_t, coord_t>::operator()(coord_t i)
  {
    precondition(has(i));
    return buffer_[i - min_];
  }

  template <typename value_t, typename coord_t>
  bool array1d<value_t, coord_t>::has(coord_t i) const
  {
    return
      i >= min_ and i <= max_;
  }

  template <typename value_t, typename coord_t>
  size_t array1d<value_t, coord_t>::memsize() const
  {
    return size_t(len_) * sizeof(value_t);
  }

  template <typename value_t, typename coord_t>
  void array1d<value_t, coord_t>::allocate_()
  {
    buffer_ = new value_t[size_t(len_)];
  }

  template <typename value_t, typename coord_t>
  void array1d<value_t, coord_t>::deallocate_()
  {
    precondition(buffer_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_1D_ARRAY1D_HH
