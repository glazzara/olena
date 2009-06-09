// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MAKE_VEC_HH
# define MLN_MAKE_VEC_HH

/*! \file
 *
 * \brief Routine to construct an mln::algebra::vec.
 */

# include <mln/algebra/vec.hh>
# include <mln/core/concept/function.hh>

namespace mln
{

  namespace make
  {


    /*! \brief Create an mln::algebra::vec<n,T>.
     *
     * \param[in] v_0 First coordinate.
     *
     * \return A 1D vector.
     */
    template <typename T>
    algebra::vec<1, T> vec(const T& v_0);

    /*! \brief Create an mln::algebra::vec<2,T>.
     *
     * \param[in] v_0 First coordinate.
     * \param[in] v_1 Second coordinate.
     *
     * \return A 2D vector.
     */
    template <typename T>
    algebra::vec<2, T> vec(const T& v_0, const T& v_1);

    /*! \brief Create an mln::algebra::vec<3,T>.
     *
     * \param[in] v_0 First coordinate.
     * \param[in] v_1 Second coordinate.
     * \param[in] v_2 Third coordinate.
     *
     * \return A 3D vector.
     */
    template <typename T>
    algebra::vec<3, T> vec(const T& v_0, const T& v_1, const T& v_2);

    /*! \brief Create an mln::algebra::vec<4,T>.
     *
     * \param[in] v_0 First coordinate.
     * \param[in] v_1 Second coordinate.
     * \param[in] v_2 Third coordinate.
     * \param[in] v_3 Fourth coordinate.
     *
     * \return A 4D vector.
     */
    template <typename T>
    algebra::vec<4, T> vec(const T& v_0, const T& v_1, const T& v_2, const T& v_3);


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    algebra::vec<1, T> vec(const T& v_0)
    {
      algebra::vec<1, T> tmp;
      tmp[0] = v_0;
      return tmp;
    }

    template <typename T>
    inline
    algebra::vec<2, T> vec(const T& v_0, const T& v_1)
    {
      algebra::vec<2, T> tmp;
      tmp[0] = v_0;
      tmp[1] = v_1;
      return tmp;
    }

    template <typename T>
    inline
    algebra::vec<3, T> vec(const T& v_0, const T& v_1, const T& v_2)
    {
      algebra::vec<3, T> tmp;
      tmp[0] = v_0;
      tmp[1] = v_1;
      tmp[2] = v_2;
      return tmp;
    }

    template <typename T>
    inline
    algebra::vec<4, T> vec(const T& v_0, const T& v_1, const T& v_2, const T& v_3)
    {
      algebra::vec<4, T> tmp;
      tmp[0] = v_0;
      tmp[1] = v_1;
      tmp[2] = v_2;
      tmp[3] = v_3;
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace make

} // end of namespace mln

#endif // ! MLN_MAKE_VEC_HH
