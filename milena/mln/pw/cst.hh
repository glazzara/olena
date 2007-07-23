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

#ifndef MLN_PW_CST_HH
# define MLN_PW_CST_HH

/*! \file mln/pw/cst.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>


namespace mln
{

  namespace pw
  {

    // FIXME: Doc!

    template <typename T>
    struct cst_ : public Function_p2v< cst_<T> >
    {
      typedef T result;

      cst_(T t);

      template <typename P>
      T operator()(const P&) const;

    private:
      T t_;
    };


    // FIXME: Doc!

    template <typename T>
    cst_<T> cst(T t);


# ifndef MLN_INCLUDE_ONLY

    // pw::cst_<T>

    template <typename T>
    cst_<T>::cst_(T t)
      : t_(t)
    {
    }

    template <typename T>
    template <typename P>
    T
    cst_<T>::operator()(const P&) const
    {
      return t_;
    }

    // pw::cst(t)

    template <typename T>
    cst_<T> cst(T t)
    {
      cst_<T> tmp(t);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::pw

} // end of namespace mln


#endif // ! MLN_PW_CST_HH
