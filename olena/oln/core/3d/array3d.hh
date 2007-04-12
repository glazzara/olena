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

#ifndef OLN_CORE_3D_ARRAY3D_HH
# define OLN_CORE_3D_ARRAY3D_HH

# include <cstddef>
# include <iostream>
# include <mlc/contract.hh>


namespace oln
{

  /// General 3D array class.

  template <typename T, typename C = int>
  class array3d_
  {
  public:

    // Not impled.
    array3d_();
    array3d_(const array3d_<T,C>& rhs);
    void operator=(const array3d_<T,C>&);
    // end of Not impled.


    /// Ctor.
    array3d_(C imin, C jmin, C kmin,
	     C imax, C jmax, C kmax);

    /// Ctor.
    array3d_(C ilen, C jlen, C klen);

    /// Dtor.
    ~array3d_();

    const T& operator()(C i, C j, C k) const;
    T& operator()(C i, C j, C k);

    const T& operator[](std::size_t ind) const;
    T& operator[](std::size_t ind);

    bool has(C i, C j, C k) const;

    std::size_t memsize() const;
    std::size_t ncells() const;

    C imin() const;
    C jmin() const;
    C kmin() const;

    C imax() const;
    C jmax() const;
    C kmax() const;

    C ilen() const;
    C jlen() const;
    C klen() const;

    const T* buffer() const;
    T* buffer();

    std::size_t i_pad() const;
    std::size_t j_pad() const;

  protected:

    C imin_, jmin_, kmin_, imax_, jmax_, kmax_;
    C ilen_, jlen_, klen_;
    std::size_t blen_;
    T*   buffer_;
    T**  array_tmp_;
    T*** array_;

  private:

    void allocate_();
    void deallocate_();
  };


  template <typename T, typename C>
  std::ostream& operator<<(std::ostream& ostr, const array3d_<T,C>& arr);


# ifndef OLN_INCLUDE_ONLY

  template <typename T, typename C>
  array3d_<T, C>::array3d_(C imin, C jmin, C kmin,
			   C imax, C jmax, C kmax) :
    imin_(imin), jmin_(jmin), kmin_(kmin),
    imax_(imax), jmax_(jmax), kmax_(kmax)
  {
    precondition(imax >= imin and jmax >= jmin and kmax >= kmin);
    ilen_ = imax - imin + 1;
    jlen_ = jmax - jmin + 1;
    klen_ = kmax - kmin + 1;
    // FIXME: Test that ilen_ and jlen_ are not huge!
    allocate_();
  }

  template <typename T, typename C>
  array3d_<T, C>::array3d_(C ilen, C jlen, C klen) :
    imin_(0),    jmin_(0),    kmin_(0),
    ilen_(ilen), jlen_(jlen), klen_(klen)
  {
    precondition(ilen > 0 and jlen > 0 and klen > 0);
    // FIXME: Test that *len_ are not huge!
    imax_ = imin_ + ilen_ - 1;
    jmax_ = jmin_ + ilen_ - 1;
    kmax_ = kmin_ + klen_ - 1;
    allocate_();
  }

  template <typename T, typename C>
  array3d_<T, C>::~array3d_()
  {
    deallocate_();
  }

  template <typename T, typename C>
  const T& array3d_<T, C>::operator()(C i, C j, C k) const
  {
    precondition(has(i, j, k));
    return array_[i][j][k];
  }

  template <typename T, typename C>
  T& array3d_<T, C>::operator()(C i, C j, C k)
  {
    precondition(has(i, j, k));
    return array_[i][j][k];
  }

  template <typename T, typename C>
  const T& array3d_<T, C>::operator[](std::size_t ind) const
  {
    precondition(buffer_ != 0);
    precondition(ind < blen_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  T& array3d_<T, C>::operator[](std::size_t ind)
  {
    precondition(buffer_ != 0);
    precondition(ind < blen_);
    return buffer_[ind];
  }

  template <typename T, typename C>
  bool array3d_<T, C>::has(C i, C j, C k) const
  {
    return
      i >= imin_ and i <= imax_ and
      j >= jmin_ and j <= jmax_ and
      k >= kmin_ and k <= kmax_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::imin() const
  {
    return imin_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::jmin() const
  {
    return jmin_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::kmin() const
  {
    return kmin_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::imax() const
  {
    return imax_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::jmax() const
  {
    return jmax_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::kmax() const
  {
    return kmax_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::ilen() const
  {
    return ilen_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::jlen() const
  {
    return jlen_;
  }

  template <typename T, typename C>
  C array3d_<T, C>::klen() const
  {
    return klen_;
  }

  template <typename T, typename C>
  const T* array3d_<T, C>::buffer() const
  {
    return buffer_;
  }

  template <typename T, typename C>
  T* array3d_<T, C>::buffer()
  {
    return buffer_;
  }

  template <typename T, typename C>
  std::size_t array3d_<T, C>::i_pad() const
  {
    precondition(buffer_ != 0 and array_ != 0);
    return std::size_t(klen_) * std::size_t(jlen_);
  }

  template <typename T, typename C>
  std::size_t array3d_<T, C>::j_pad() const
  {
    precondition(buffer_ != 0 and array_ != 0);
    return klen_;
  }

  template <typename T, typename C>
  std::size_t array3d_<T, C>::ncells() const
  {
    return blen_;
  }

  template <typename T, typename C>
  std::size_t array3d_<T, C>::memsize() const
  {
    return
      // buffer_
      blen_ * sizeof(T)
      +
      // array_tmp_
      std::size_t(ilen_) * std::size_t(jlen_) * sizeof(T*)
      +
      // array_
      std::size_t(ilen_) * sizeof(T**);
  }

  template <typename T, typename C>
  void array3d_<T, C>::allocate_()
  {
    blen_ = std::size_t(ilen_) * std::size_t(jlen_) * std::size_t(klen_);
    buffer_    = new T  [blen_];
    array_tmp_ = new T* [std::size_t(ilen_) * size_t(jlen_)];
    array_     = new T**[std::size_t(ilen_)];
    T* buf = buffer_ - kmin_;
    for (C i = 0; i < ilen_; ++i)
      {
 	T** arr_i_ = array_tmp_ + i * jlen_;
	array_[i] = arr_i_ - jmin_;
	for (C j = 0; j < jlen_; ++j)
	  {
	    arr_i_[j] = buf;
	    buf += klen_;
	  }
      }
    array_ -= imin_;
  }

  template <typename T, typename C>
  void array3d_<T, C>::deallocate_()
  {
    precondition(buffer_ != 0 and array_ != 0 and array_tmp_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
    delete[] array_tmp_;
    array_tmp_ = 0;  // safety
    array_ += imin_;
    delete[] array_;
    array_ = 0;  // safety
  }

  template <typename T, typename C>
  std::ostream& operator<<(std::ostream& ostr, const array3d_<T,C>& arr)
  {
    return ostr << "{"
		<< " imin=" << arr.imin()
		<< " jmin=" << arr.jmin()
		<< " kmin=" << arr.kmin()
		<< " imax=" << arr.imax()
		<< " jmax=" << arr.jmax()
		<< " kmax=" << arr.kmax()
		<< " ilen=" << arr.ilen()
		<< " jlen=" << arr.jlen()
		<< " klen=" << arr.klen()
		<< " blen=" << arr.ncells() << " }";
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_3D_ARRAY3D_HH
