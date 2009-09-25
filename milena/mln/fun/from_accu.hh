// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///
/// Wrap an accumulator into a function.


#ifndef MLN_FUN_FROM_ACCU_HH
# define MLN_FUN_FROM_ACCU_HH

# include <mln/fun/unary_param.hh>
# include <mln/core/concept/accumulator.hh>


namespace mln
{

  namespace fun
  {


    /*! \brief Wrap an accumulator into a function.
     */
    template <typename A>
    struct from_accu : unary_param<from_accu<A>, A*>
    {
      from_accu();
      from_accu(A* a);
    };

  } // end of namespace mln::fun

  namespace trait
  {

    namespace next
    {
      template <typename A, typename T>
      struct set_unary_<mln::fun::from_accu<A>, mln::Object, T>
      {
	typedef set_unary_           ret;
	typedef typename A::result   result;
	typedef typename A::argument argument;
	typedef A* param_t;

	static result read(const param_t& accu_, const argument& x);

      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait



# ifndef MLN_INCLUDE_ONLY


  namespace fun
  {

    template <typename A>
    inline
    from_accu<A>::from_accu()
      : unary_param<from_accu<A>, A*>()
    {

    }


    template <typename A>
    inline
    from_accu<A>::from_accu(A* a)
      : unary_param<from_accu<A>, A*>(a)
    {

    }

  } // end of namespace mln::fun



  namespace trait
  {

    namespace next
    {

      template <typename A, typename T>
      inline
      typename set_unary_<mln::fun::from_accu<A>, mln::Object, T>::result
      set_unary_<mln::fun::from_accu<A>,
		 mln::Object, T>::read(const param_t& accu_,
				       const argument& x)
      {
	mln_precondition(accu_ != 0);

	accu_->take(x);
	  return accu_->to_result ();
      }

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait



# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_FUN_FROM_ACCU_HH
