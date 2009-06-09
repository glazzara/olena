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

#ifndef MLN_VALUE_SHELL_HH
# define MLN_VALUE_SHELL_HH

/// \file
///
/// \brief Define a shell type that encloses a value.

# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  // Forward declaration.
  namespace value {
    template <typename F, typename I> struct shell;
  }

  namespace value
  {

    namespace impl
    {

      template <typename F, typename I, class C>
      struct shell_ { };


      template <typename F, typename I>
      struct shell_<F, I, Function_v2v<void> >
      {
	const mln_value(I)&
	set_(I& ima, const mln_site(I)& s, mln_result(F) v);
      };

    } // end of namespace mln::value::impl


    template <typename F, typename I>
    struct shell
      : public Proxy< shell<F,I> >,
	public mln::internal::proxy_impl< mln_result(F), shell<F,I> >,
	public impl::shell_<F, I, typename F::category>
    {
      typedef mln_result(F) value;

      // Ctor
      shell(Image<I> &ima, const mln_site(I) &s);

      // Write
      value operator=(value);

      // Enc
      typedef void enc; // FIXME

      // Equiv
      typedef value equiv;

      mln_result(F) subj_();

      // <<
      // std::ostream& operator<<(std::ostream& ostr);

      // >>
      // std::istream& operator>>(std::istream& istr);


    protected:
      I& ima_;
      mln_site(I) s_;
      mln_result(F) v_;
    };


# ifndef MLN_INCLUDE_ONLY

    // Constructor.
    template <typename F, typename I>
    shell<F,I>::shell(Image<I>& ima, const mln_site(I)& s)
      :
      ima_(exact(ima)),
      s_(s),
      v_(F()(exact(ima)(s)))
    {
    }

    // Write for everyone
    template <typename F, typename I>
    typename F::result
    shell<F,I>::operator=(mln_result(F) v)
    {
      v_ = F()(set_(ima_, s_, v));
      return v_;
    }

    template <typename F, typename I>
    mln_result(F)
    shell<F,I>::subj_()
    {
      return v_;
    }


    namespace impl
    {

//      template <typename F, typename I>
//      const mln_value(I)&
//      shell_<F, I, Function_v2v<void> >::set_(I& ima,
//						const mln_site(I)& s,
//						mln_result(F) v)
//      {
//	ima(s) = F().f_1(v);
//	return ima(s);
//      }

      template <typename F, typename I>
      const mln_value(I)&
      shell_<F, I, Function_v2v<void> >::set_(I& ima,
						  const mln_site(I)& s,
						  mln_result(F) v)
      {
	ima(s) = F().f_1(v, ima(s));
	return ima(s);
      }

    } // end of namespace mln::value::impl


# endif // MLN_INCLUDE_ONLY


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_SHELL_HH
