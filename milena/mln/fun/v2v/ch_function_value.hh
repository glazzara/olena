// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_FUN_V2V_CH_FUNCTION_VALUE_HH
# define MLN_FUN_V2V_CH_FUNCTION_VALUE_HH

/// \file
///
/// Wrap a function v2v and convert its result to another type.

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/ch_function_value_impl.hh>



namespace mln
{

  // Forward declaration.
  namespace convert { template <typename T, typename O> T to(const O& from); }

  namespace fun
  {

    namespace v2v
    {

      /// \brief Wrap a function v2v and convert its result to another type.
      template <typename F, typename V>
      class ch_function_value : public Function_v2v< ch_function_value<F,V> >,
				public internal::ch_function_value_impl<F, V>
      {
      public:
	typedef F function;
	typedef V result;
	typedef V rvalue;
	typedef V lvalue;

	ch_function_value();
	ch_function_value(const F& f);

	template <typename W>
	V operator()(const W& w) const;

      private:
	F f_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename V>
      inline
      ch_function_value<F,V>::ch_function_value()
      {
      }

      template <typename F, typename V>
      inline
      ch_function_value<F,V>::ch_function_value(const F& f)
      {
	f_ = f;
      }

      template <typename F, typename V>
      template <typename W>
      inline
      V
      ch_function_value<F,V>::operator()(const W& w) const
      {
	return mln::convert::to<V>(f_(w));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_CH_FUNCTION_VALUE_HH
