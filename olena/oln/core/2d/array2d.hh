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

#ifndef OLN_CORE_2D_ARRAY2D_HH
# define OLN_CORE_2D_ARRAY2D_HH

# include <cstddef>
# include <mlc/contract.hh>
# include <oln/core/abstract/array.hh>


namespace oln
{

  /// General 2D array class.
  template <typename value_t, typename coord_t = int>
  class array2d : public abstract::array
  {
  public:

    /// Ctor.
    array2d(coord_t imin, coord_t jmin, coord_t imax, coord_t jmax);
    /// Ctor.
    array2d(coord_t ilen, coord_t jlen);

    /// Dtor.
    ~array2d();

    value_t  operator()(coord_t i, coord_t j) const;
    value_t& operator()(coord_t i, coord_t j);

    bool has(coord_t i, coord_t j) const;

    std::size_t memsize() const;

  protected:

    coord_t   imin_, jmin_, imax_, jmax_;
    coord_t   ilen_, jlen_;
    value_t*  buffer_;
    value_t** array_;

  private:

    void allocate_();
    void deallocate_();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t imin, coord_t jmin,
				     coord_t imax, coord_t jmax) :
    imin_(imin),
    jmin_(jmin),
    imax_(imax),
    jmax_(jmax)
  {
    precondition(imax >= imin and jmax >= jmin);
    ilen_ = imax - imin + 1;
    jlen_ = jmax - jmin + 1;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t ilen, coord_t jlen) :
    imin_(0),
    jmin_(0),
    ilen_(ilen),
    jlen_(jlen)
  {
    precondition(ilen > 0 and jlen > 0);
    imax_ = imin_ + ilen_;
    jmax_ = jmin_ + ilen_;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::~array2d()
  {
    deallocate_();
  }

  template <typename value_t, typename coord_t>
  value_t array2d<value_t, coord_t>::operator()(coord_t i, coord_t j) const
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  value_t& array2d<value_t, coord_t>::operator()(coord_t i, coord_t j)
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  bool array2d<value_t, coord_t>::has(coord_t i, coord_t j) const
  {
    return
      i >= imin_ and i <= imax_ and
      j >= jmin_ and j <= jmax_;
  }

  template <typename value_t, typename coord_t>
  size_t array2d<value_t, coord_t>::memsize() const
  {
    return
      // buffer_
      size_t(ilen_) * size_t(jlen_) * sizeof(value_t)
      +
      // array_
      size_t(ilen_) * sizeof(value_t*);
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::allocate_()
  {
    buffer_ = new value_t[size_t(ilen_) * size_t(jlen_)];
    array_ = new value_t*[size_t(ilen_)];
    value_t* buf = buffer_ - jmin_;
    for (coord_t i = 0; i < ilen_; ++i)
      {
	array_[i] = buf;
	buf += jlen_;
      }
    array_ -= imin_;
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::deallocate_()
  {
    precondition(buffer_ != 0 and array_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
    array_ += imin_;
    delete[] array_;
    array_ = 0;  // safety
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_2D_ARRAY2D_HH
