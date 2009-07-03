// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_UNARY_HH
# define MLN_FUN_UNARY_HH

# include <mln/core/concept/meta_function.hh>
# include <mln/fun/spe/unary.hh>
# include <mln/trait/next/solve.hh>


namespace mln
{

  namespace fun
  {

    // Forward declarations, for composition with unary::operator()(Fun)
    struct compose;

    namespace internal
    {

      template <typename U>
      struct unary_with {};

    }

    template <typename F, typename E = F>
    struct unary: mln::Meta_Function_v2v< E >
    {
      typedef F flag;
      typedef mln_trait_fun_param(flag) param;
      typedef mln_trait_fun_storage(flag) storage;

      template <typename T>
      struct with {
	typedef mln_trait_nunary(internal::unary_with<F>, T) def;
	typedef typename def::ret ret;
      };

      template <typename T>
      typename with<T>::ret::result operator()(const T& v) const
      {
	return with<T>::def::call(exact(*this), v);
      }

      template <typename T>
      typename with<T>::ret::template lresult_with<T>::ret operator()(T& v) const
      {
	// See the commentary in next method.
	typedef typename with<T>::ret fun_t;
	fun_t f(state());
	return f.apply_rw(v);
      }

      template <typename T, typename R>
      void set(T& v, const R& r) const
      {
	// Decomposing "with<T>::ret(state()).set(v, r)" into 3 lines
	// helps g++-3.3!
	typedef typename with<T>::ret fun_t;
	fun_t f(state());
	f.set(v, r);
      }

      template <typename U>
      void init(const U& value)
      {
	state_ = mln::trait::function::internal::introspect::has_storage_t<flag, void>::compute(value);
      };

      unary()
      {
      }

      template <typename U>
      unary(const U& param)
      {
	this->init(param);
      }

      stored<storage>& state()
      {
	return state_;
      }

      const stored<storage>& state() const
      {
	return state_;
      }

    protected:
      stored<storage> state_;
    };

  } // end of namespace mln::fun

  namespace trait
  {

    namespace next
    {

      // Any type
      template <typename F, typename T>
      struct set_unary_< mln::fun::internal::unary_with<F>, mln::Object, T>
      {
	struct ret_t
	{
	  typedef mln::fun::spe::unary<F, T> ret;

	  static typename ret::result call(const F& f, const T& v)
	  {
	    return ret(f.state())(v);
	  }
	};

	typedef ret_t ret;
      };

      // Meta Function
      template <typename F, typename G>
      struct set_unary_< mln::fun::internal::unary_with<F>, mln::Meta_Function, G>
      {
	// FIXME: Workaround for cyclic references (unary -> unary_with -> compose -> unary)
	template <typename T>
	struct identity
	{
	  typedef T ret;
	};

	struct ret_t
	{
	  typedef typename identity<mln::fun::compose>::ret::template with<F, G>::ret ret;

	  static typename ret::result call(const F& f, const G& g)
	  {
	    return ret()(f, g);
	  }

	};

	typedef ret_t ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_FUN_UNARY_HH
