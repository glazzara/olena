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

#ifndef MLN_FUN_P2V_TERNARY_HH
# define MLN_FUN_P2V_TERNARY_HH

/*! \file mln/fun/p2v/ternary.hh
 *
 * \brief FIXME.
 */

# include <mln/fun/internal/selector.hh>


namespace mln
{

  namespace fun
  {

    namespace p2v
    {

      template <typename P, typename T, typename F>
      struct ternary_
	: fun::internal::selector_p2_<mln_result(T), ternary_<P,T,F> >::ret
      {
	typedef mln_result(T) result;

	ternary_(const Function_p2b<P>& f_pred,
		 const Function_p2v<T>& f_true,
		 const Function_p2v<F>& f_false);

	template <typename Pt>
	result operator()(const Pt& p) const;

      protected:
	const P f_pred_;
	const T f_true_;
	const F f_false_;
      };
    
    
      template <typename P, typename T, typename F>
      ternary_<P, T, F>
      ternary(const Function_p2b<P>& f_pred,
	      const Function_p2v<T>& f_true,
	      const Function_p2v<F>& f_false);
    

# ifndef MLN_INCLUDE_ONLY

      template <typename P, typename T, typename F>
      ternary_<P,T,F>::ternary_(const Function_p2b<P>& f_pred,
				const Function_p2v<T>& f_true,
				const Function_p2v<F>& f_false)
	: f_pred_(exact(f_pred)),
	  f_true_(exact(f_true)),
	  f_false_(exact(f_false))
      {
      }

      template <typename P, typename T, typename F>
      template <typename Pt>
      mln_result(T)
	ternary_<P,T,F>::operator()(const Pt& p) const
      {
	return f_pred_(p) ? f_true_(p) : f_false_(p);
      }


      template <typename P, typename T, typename F>
      ternary_<P, T, F>
      ternary(const Function_p2b<P>& f_pred,
	      const Function_p2v<T>& f_true,
	      const Function_p2v<F>& f_false)
      {
	ternary_<P, T, F> tmp(exact(f_pred),
			      exact(f_true),
			      exact(f_false));
	return tmp;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2V_TERNARY_HH
