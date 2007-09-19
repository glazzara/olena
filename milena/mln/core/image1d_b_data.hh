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

#ifndef MLN_IMAGE1D_B_DATA_HH
# define MLN_IMAGE1D_B_DATA_HH

/*! \file mln/core/image1d_b_data.hh
 *
 * \brief Definition of the basic mln::image1d_b_data class.
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/box1d.hh>

# include <mln/fun/i2v/all.hh>

namespace mln
{
  template <typename T>
  struct image1d_b_data
  {
  public:
    image1d_b_data(const box1d& b, unsigned bdr);
    ~image1d_b_data();

    T*  buffer_;
    T* array_;

    box1d b_;  // theoretical box
    unsigned bdr_;
    box1d vb_; // virtual box, i.e., box including the virtual border

    void update_vb_();
    void allocate_();
    void deallocate_();

  };


  template <typename T>
  image1d_b_data<T>::image1d_b_data(const box1d& b, unsigned bdr)
    : buffer_(0),
      array_ (0),
      b_     (b),
      bdr_   (bdr)
  {
    allocate_();
  }

  template <typename T>
  image1d_b_data<T>::~image1d_b_data()
  {
    deallocate_();
  }


  // private

  template <typename T>
  void
  image1d_b_data<T>::update_vb_()
  {
    vb_.pmin() = b_.pmin() - dpoint1d(all(bdr_));
    vb_.pmax() = b_.pmax() + dpoint1d(all(bdr_));
  }

  template <typename T>
  void
  image1d_b_data<T>::allocate_()
  {
    update_vb_();
    unsigned
      ni = vb_.len(0);
    buffer_ = new T[ni];
    array_ = buffer_ - vb_.pmin().ind();
    mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
  }

  template <typename T>
  void
  image1d_b_data<T>::deallocate_()
  {
    if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
  }

}

#endif // ! MLN_IMAGE1D_B_DATA_HH
