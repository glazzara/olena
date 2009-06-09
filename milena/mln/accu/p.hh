// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_P_HH
# define MLN_ACCU_P_HH

/// \file
///
/// Define an accumulator that computes a min and a max.

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  namespace accu
  {


    ///Generic p of accumulators.
    ///
    /// The parameter \c V is the type of values.
    template <typename A>
    struct p : public mln::accu::internal::base< const mln_result(A)& , p<A> >
    {
      typedef mln_argument(A)  argument;

      p();
      p(const A& a);

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);
      void take(const argument& t);
      void take(const p<A>& other);
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

      /// Meta accumulator for p.

      template <typename mA>
      struct p : public Meta_Accumulator< p<mA> >
      {
	template <typename V>
	struct with
	{
	  typedef p<V> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename A>
    inline
    p<A>::p()
    {
      init();
    }

    template <typename A>
    inline
    p<A>::p(const A& a)
      : a_(a)
    {
      init();
    }

    template <typename A>
    inline
    void
    p<A>::init()
    {
      a_.init();
    }

    template <typename A>
    inline
    void
    p<A>::take_as_init_(const argument& t)
    {
      a_.take_as_init_(t.p()); // FIXME: Generalize with "psite(t)".
    }

    template <typename A>
    inline
    void
    p<A>::take(const argument& t)
    {
      a_.take(t.p());
    }

    template <typename A>
    inline
    void
    p<A>::take(const p<A>& other)
    {
      a_.take(other.a_);
    }

    template <typename A>
    inline
    const mln_result(A)&
    p<A>::to_result() const
    {
      return a_.to_result();
    }

    template <typename A>
    inline
    bool
    p<A>::is_valid() const
    {
      return a_.is_valid();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_P_HH
