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

#ifndef MLN_FUN_P2V_ELIFS_HH
# define MLN_FUN_P2V_ELIFS_HH

/*! \file mln/fun/p2v/elifs.hh
 *
 * \brief FIXME.
 */

# include <mln/fun/p2v/ternary.hh>


namespace mln
{

  namespace fun
  {

    namespace p2v
    {

      template <typename T1, typename N1,
		typename T2, typename N2,
		typename O>
      ternary_<T1, N1,
	       ternary_<T2, N2, O> >
      elifs(const Function_p2b<T1>& f_if_1, const Function_p2v<N1>& f_then_1,
	    const Function_p2b<T2>& f_if_2, const Function_p2v<N2>& f_then_2,
	    const Function_p2v<O>&  f_otherwise);


# ifndef MLN_INCLUDE_ONLY

      template <typename T1, typename N1,
		typename T2, typename N2,
		typename O>
      inline
      ternary_<T1, N1,
	       ternary_<T2, N2, O> >
      elifs(const Function_p2b<T1>& f_if_1, const Function_p2v<N1>& f_then_1,
	    const Function_p2b<T2>& f_if_2, const Function_p2v<N2>& f_then_2,
	    const Function_p2v<O>&  f_otherwise)
      {
	typedef ternary_<T2, N2, O> T2_N2_O;
	T2_N2_O f_otherwise_1(f_if_2, f_then_2, f_otherwise);
	ternary_<T1, N1, T2_N2_O> tmp(f_if_1, f_then_1, f_otherwise_1);
	return tmp;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2V_ELIFS_HH
