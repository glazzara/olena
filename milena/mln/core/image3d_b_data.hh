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

#ifndef MLN_IMAGE3D_B_DATA_HH
# define MLN_IMAGE3D_B_DATA_HH

/*! \file mln/core/image3d_b_data.hh
 *
 * \brief Definition of the basic mln::image3d_b_data class.
 */

# include <mln/core/box3d.hh>
# include <mln/core/point.hh>

# include <mln/fun/i2v/all.hh>

namespace mln
{
  template <typename T>
  struct image3d_b_data
  {
  public:
    image3d_b_data(const box3d& b, unsigned bdr);
    ~image3d_b_data();

    T*  buffer_;
    T*** array_;

    box3d b_;  // theoretical box
    unsigned bdr_;
    box3d vb_; // virtual box, i.e., box including the virtual border

    void update_vb_();
    void allocate_();
    void deallocate_();

  };


  template <typename T>
  image3d_b_data<T>::image3d_b_data(const box3d& b, unsigned bdr)
    : buffer_(0),
      array_ (0),
      b_     (b),
      bdr_   (bdr)
  {
    allocate_();
  }

  template <typename T>
  image3d_b_data<T>::~image3d_b_data()
  {
    deallocate_();
  }

  template <typename T>
  void
  image3d_b_data<T>::update_vb_()
  {
    vb_.pmin() = b_.pmin() - dpoint3d(all(bdr_));
    vb_.pmax() = b_.pmax() + dpoint3d(all(bdr_));
  }

  template <typename T>
  void
  image3d_b_data<T>::allocate_()
  {
    update_vb_();
    unsigned
      ns = vb_.len(0),
      nr = vb_.len(1),
      nc = vb_.len(2);
    buffer_ = new T[nr * nc * ns];
    array_ = new T**[ns];
    T* buf = buffer_ - vb_.pmin().col();
    for (unsigned i = 0; i < ns; ++i)
    {
      T** tmp = new T*[nr];
      array_[i] = tmp;
      for (unsigned j = 0; j < nr; ++j)
      {
	array_[i][j] = buf;
	buf += nc;
      }
      array_[i] -= vb_.pmin().row();
    }
    array_ -= vb_.pmin().sli();
    mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
  }

  template <typename T>
  void
  image3d_b_data<T>::deallocate_()
  {
    if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
    for (typename point3d::coord i = vb_.pmin().sli(); i <= vb_.pmax().sli(); ++i)
      {
	if (array_[i])
	  {
	    array_[i] += vb_.pmin().row();
	    delete[] array_[i];
	    array_[i] = 0;
	  }
      }
    if (array_)
      {
	array_ += vb_.pmin().sli();
	delete[] array_;
	array_ = 0;
      }
  }

}

#endif // ! MLN_IMAGE3D_B_DATA_HH
