// Copyright (C) 2001, 2003, 2004, 2006, 2007 EPITA Research and
// Development Laboratory
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
# include <iostream>
# include <mlc/contract.hh>


namespace oln
{

  /// General 2D array class.

  template <typename T, typename C = int>
  class array2d_
  {
  public:

    // Not impled.
    array2d_();
    array2d_(const array2d_<T,C>& rhs);
    void operator=(const array2d_<T,C>&);
    // end of Not impled.


    /// Ctor.
    array2d_(C imin, C jmin, C imax, C jmax);

    /// Ctor.
    array2d_(C ilen, C jlen);

    /// Dtor.
    ~array2d_();

    const T& operator()(C i, C j) const;
          T& operator()(C i, C j);

    const T& operator[](std::size_t ind) const;
          T& operator[](std::size_t ind);

    bool has(C i, C j) const;

    std::size_t memsize() const;
    std::size_t ncells() const;

    C imin() const;
    C jmin() const;
    C imax() const;
    C jmax() const;

    const T* buffer() const;
    T* buffer();

    std::size_t i_pad() const;

  protected:

    C   imin_, jmin_, imax_, jmax_;
    C   ilen_, jlen_;
    std::size_t blen_;
    T*  buffer_;
    T** array_;

  private:

    void allocate_();
    void deallocate_();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename T, typename C>
  array2d_<T, C>::array2d_(C imin, C jmin, C imax, C jmax) :
    imin_(imin), jmin_(jmin),
    imax_(imax), jmax_(jmax)
  {
    precondition(imax >= imin and jmax >= jmin);
    ilen_ = imax - imin + 1;
    jlen_ = jmax - jmin + 1;
    // FIXME: Test that ilen_ and jlen_ are not huge!
    allocate_();
  }

  template <typename T, typename C>
  array2d_<T, C>::array2d_(C ilen, C jlen) :
    imin_(0),    jmin_(0),
    ilen_(ilen), jlen_(jlen)
  {
    precondition(ilen > 0 and jlen > 0);
    // FIXME: Test that ilen_ and jlen_ are not huge!
    imax_ = imin_ + ilen_ - 1;
    jmax_ = jmin_ + ilen_ - 1;
    allocate_();
  }

  template <typename T, typename C>
  array2d_<T, C>::~array2d_()
  {
    deallocate_();
  }

  template <typename T, typename C>
  const T& array2d_<T, C>::operator()(C i, C j) const
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename T, typename C>
  T& array2d_<T, C>::operator()(C i, C j)
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename T, typename C>
  const T& array2d_<T, C>::operator[](std::size_t ind) const
  {
    precondition(buffer_ != 0);
    precondition(ind < blen_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  T& array2d_<T, C>::operator[](std::size_t ind)
  {
    precondition(buffer_ != 0);
    precondition(ind < blen_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  bool array2d_<T, C>::has(C i, C j) const
  {
    return
      i >= imin_ and i <= imax_ and
      j >= jmin_ and j <= jmax_;
  }

  template <typename T, typename C>
  C array2d_<T, C>::imin() const
  {
    return imin_;
  }

  template <typename T, typename C>
  C array2d_<T, C>::jmin() const
  {
    return jmin_;
  }

  template <typename T, typename C>
  C array2d_<T, C>::imax() const
  {
    return imax_;
  }

  template <typename T, typename C>
  C array2d_<T, C>::jmax() const
  {
    return jmax_;
  }

  template <typename T, typename C>
  const T* array2d_<T, C>::buffer() const
  {
    return buffer_;
  }

  template <typename T, typename C>
  T* array2d_<T, C>::buffer()
  {
    return buffer_;
  }

  template <typename T, typename C>
  std::size_t array2d_<T, C>::i_pad() const
  {
    precondition(buffer_ != 0 and array_ != 0);
    return jlen_;
  }

  template <typename T, typename C>
  std::size_t array2d_<T, C>::ncells() const
  {
    return blen_;
  }

  template <typename T, typename C>
  std::size_t array2d_<T, C>::memsize() const
  {
    return
      // buffer_
      blen_ * sizeof(T)
      +
      // array_
      std::size_t(ilen_) * sizeof(T*);
  }

  template <typename T, typename C>
  void array2d_<T, C>::allocate_()
  {
    blen_ = std::size_t(ilen_) * std::size_t(jlen_);
    buffer_ = new T[blen_];
    array_  = new T*[std::size_t(ilen_)];
    T* buf = buffer_ - jmin_;
    for (C i = 0; i < ilen_; ++i)
      {
	array_[i] = buf;
	buf += jlen_;
      }
    array_ -= imin_;
  }

  template <typename T, typename C>
  void array2d_<T, C>::deallocate_()
  {
    precondition(buffer_ != 0 and array_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
    array_ += imin_;
    delete[] array_;
    array_ = 0;  // safety
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_2D_ARRAY2D_HH
