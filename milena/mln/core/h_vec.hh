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

#ifndef MLN_CORE_H_VEC_HH
# define MLN_CORE_H_VEC_HH

/*! \file mln/core/h_vec.hh
 *
 * \brief Definition of the mln::h_vec alias and of its
 * construction routine.
 */

# include <mln/metal/vec.hh>


namespace mln
{


  template <unsigned dim, typename T>
  struct h_vec : public metal::vec<dim + 1, T>
  {
      h_vec()
	: metal::vec<dim + 1, T>(make::vec<dim + 1, T>(0))
      {
	this->data_[dim] = 1;
      }

      h_vec(const metal::vec<dim, T>& x);

      operator metal::vec<dim, T>() const;
  };

# ifndef MLN_INCLUDE_ONLY

  template <unsigned dim, typename T>
  h_vec<dim,T>::h_vec(const metal::vec<dim, T>& x)
  {
    for (unsigned i = 0; i < dim; ++i)
      this->data_[i] = x[i];
    this->data_[dim] = 1;
  }

  template <unsigned dim, typename T>
  h_vec<dim,T>::operator metal::vec<dim,T>() const
  {
    metal::vec<dim,T> x;
    for (unsigned i = 0; i < dim; ++i)
      x[i] = this->data_[i] / this->data_[dim];
    return x;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_H_VEC_HH
