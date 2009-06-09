// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_V_HH
# define MLN_ACCU_V_HH

/// \file
///
/// Define an accumulator that computes a min and a max.


# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {



    /// Generic val of accumulators.
    template <typename A>
    struct val : public mln::accu::internal::base< const mln_result(A)& , val<A> >
    {
      typedef mln_argument(A)  argument;

      val();
      val(const A& a);

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);
      void take(const argument& t);
      void take(const val<A>& other);
      template <typename I>
      void take_as_init_(const util::pix<I>& pix);
      template <typename I>
      void take(const util::pix<I>& pix);
      /// \}

      /// Get the value of the accumulator.
      const mln_result(A)& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      A a_;
    };


    namespace meta
    {

      /// Meta accumulator for val.

      template <typename mA>
      struct val : public Meta_Accumulator< val<mA> >
      {
	template <typename V>
	struct with
	{
	  typedef mln_accu_with(mA, mln_value(V)) A;
	  typedef val<A> ret;
	};
      };

    }

# ifndef MLN_INCLUDE_ONLY

    template <typename A>
    inline
    val<A>::val()
    {
      init();
    }

    template <typename A>
    inline
    val<A>::val(const A& a)
      : a_(a)
    {
      init();
    }

    template <typename A>
    inline
    void
    val<A>::init()
    {
      a_.init();
    }

    template <typename A>
    inline
    void
    val<A>::take_as_init_(const argument& t)
    {
      a_.take_as_init_(t);
    }

    template <typename A>
    inline
    void
    val<A>::take(const argument& t)
    {
      a_.take(t);
    }

    template <typename A>
    inline
    void
    val<A>::take(const val<A>& other)
    {
      a_.take(other.a_);
    }

    template <typename A>
    template <typename I>
    inline
    void
    val<A>::take_as_init_(const util::pix<I>& pix)
    {
      a_.take_as_init_(pix.v()); // FIXME: Generalize with "value(pix)".
    }

    template <typename A>
    template <typename I>
    inline
    void
    val<A>::take(const util::pix<I>& pix)
    {
      a_.take(pix.v());
    }

    template <typename A>
    inline
    const mln_result(A)&
    val<A>::to_result() const
    {
      return a_.to_result();
    }

    template <typename A>
    inline
    bool
    val<A>::is_valid() const
    {
      return a_.is_valid();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_VAL_HH
