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

#ifndef OLN_CORE_1D_ARRAY1D_HH
# define OLN_CORE_1D_ARRAY1D_HH

# include <cstddef>
# include <mlc/contract.hh>


namespace oln
{

  /// General 1D array class.

  template <typename T, typename C = int>
  class array1d_
  {
  public:

    /// Ctor.
    array1d_(C imin, C imax);

    /// Ctor.
    array1d_(C len);

    /// Dtor.
    ~array1d_();

    const T& operator()(C i) const;
          T& operator()(C i);

    const T& operator[](std::size_t ind) const;
          T& operator[](std::size_t ind);

    bool has(C i) const;

    std::size_t memsize() const;
    std::size_t ncells() const;

    C imin() const;
    C imax() const;

    const T* buffer() const;
    T* buffer();

  protected:

    C   imin_, imax_;
    C   len_;
    T*  buffer_;

  private:

    void allocate_();
    void deallocate_();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename T, typename C>
  array1d_<T, C>::array1d_(C imin, C imax) :
    imin_(imin),
    imax_(imax)
  {
    precondition(imax >= imin);
    len_ = imax - imin + 1;
    allocate_();
  }

  template <typename T, typename C>
  array1d_<T, C>::array1d_(C len) :
    imin_(0),
    len_(len)
  {
    precondition(len > 0);
    imax_ = imin_ + len_;
    allocate_();
  }

  template <typename T, typename C>
  array1d_<T, C>::~array1d_()
  {
    deallocate_();
  }

  template <typename T, typename C>
  const T& array1d_<T, C>::operator()(C i) const
  {
    precondition(has(i));
    return buffer_[i];
  }

  template <typename T, typename C>
  T& array1d_<T, C>::operator()(C i)
  {
    precondition(has(i));
    return buffer_[i];
  }

  template <typename T, typename C>
  const T& array1d_<T, C>::operator[](std::size_t ind) const
  {
    precondition(buffer_ != 0);
    precondition(ind < len_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  T& array1d_<T, C>::operator[](std::size_t ind)
  {
    precondition(buffer_ != 0);
    precondition(ind < len_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  bool array1d_<T, C>::has(C i) const
  {
    return i >= imin_ and i <= imax_;
  }


  template <typename T, typename C>
  C array1d_<T, C>::imin() const
  {
    return imin_;
  }

  template <typename T, typename C>
  C array1d_<T, C>::imax() const
  {
    return imax_;
  }

  template <typename T, typename C>
  const T* array1d_<T, C>::buffer() const
  {
    return buffer_;
  }

  template <typename T, typename C>
  T* array1d_<T, C>::buffer()
  {
    return buffer_;
  }

  template <typename T, typename C>
  std::size_t array1d_<T, C>::ncells() const
  {
    return len_;
  }

  template <typename T, typename C>
  std::size_t array1d_<T, C>::memsize() const
  {
    return len_ * sizeof(T);
  }

  template <typename T, typename C>
  void array1d_<T, C>::allocate_()
  {
    buffer_ = new T[len_];
  }

  template <typename T, typename C>
  void array1d_<T, C>::deallocate_()
  {
    precondition(buffer_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_1D_ARRAY1D_HH
