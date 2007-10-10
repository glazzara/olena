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


  template <unsigned d, typename C>
  struct h_vec : public metal::vec<d + 1, C>
  {
    /// Dimension is the 'natural' one (3 for 3D), not the one of the vector (dim + 1).
    enum { dim = d };

    /// Constructor without argument.
    h_vec();

    /// Constructor from a metal::vec.
    h_vec(const metal::vec<d,C>& x);
    
    /// Conversion to a metal::vec.
    operator metal::vec<d,C>() const;
  };



# ifndef MLN_INCLUDE_ONLY

  template <unsigned d, typename C>
  h_vec<d,C>::h_vec()
  {
  }

  template <unsigned d, typename C>
  h_vec<d,C>::h_vec(const metal::vec<d,C>& x)
  {
    for (unsigned i = 0; i < d; ++i)
      this->data_[i] = x[i];
    this->data_[d] = 1; // FIXME: literal::one
  }

  template <unsigned d, typename C>
  h_vec<d,C>::operator metal::vec<d,C>() const
  {
    const C w = this->data_[d];
    mln_assertion(w != 0);

    metal::vec<d,C> tmp;
    for (unsigned i = 0; i < d; ++i)
      tmp[i] = this->data_[i] / w;
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_H_VEC_HH
