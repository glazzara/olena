// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ACCU_SUM_HH
# define MLN_ACCU_SUM_HH

/// \file
///
/// Define an accumulator that computes a sum.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>

# include <mln/util/pix.hh> // To prevent accu::sum to work on pixels (ambiguous).

# include <mln/trait/value_.hh>      // For mln_sum.
# include <mln/value/builtin/all.hh> // In the case of summing builtin values.
# include <mln/literal/zero.hh>      // For initialization.


namespace mln
{

  // Forward declaration.
  namespace accu { template <typename T, typename S> struct sum; }


  // Traits.

  namespace trait
  {

    template <typename T, typename S>
    struct accumulator_< accu::sum<T,S> >
    {
      typedef accumulator::has_untake::yes    has_untake;
      typedef accumulator::has_set_value::yes has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::not_ok   when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    /// \brief Generic sum accumulator class.
    ///
    /// Parameter \c T is the type of values that we sum.  Parameter \c
    /// S is the type to store the value sum; the default type of
    /// \c S is the summation type (property) of \c T.
    ///
    /// \ingroup modaccuvalues
    //
    template <typename T, typename S = mln_sum(T)>
    struct sum : public mln::accu::internal::base< const S&, sum<T,S> >
    {
      typedef T argument;

      sum();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take_as_init_(const argument& t);
      void take(const sum<T,S>& other);

      void untake(const argument& t);
      void set_value(const S& s);
      /// \}

      /// Get the value of the accumulator.
      const S& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      S s_;
    };


    template <typename I, typename S>
    struct sum< util::pix<I>, S >;

    namespace meta
    {

      /// Meta accumulator for sum.
      struct sum : public Meta_Accumulator< sum >
      {
	template <typename T, typename S = mln_sum(T)>
	struct with
	{
	  typedef accu::sum<T, S> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S>
    inline
    sum<T,S>::sum()
    {
      init();
    }

    template <typename T, typename S>
    inline
    void
    sum<T,S>::init()
    {
      s_ = literal::zero;
    }

    template <typename T, typename S>
    inline
    void sum<T,S>::take(const argument& t)
    {
      s_ += static_cast<S>(t);
    }

    template <typename T, typename S>
    inline
    void sum<T,S>::untake(const argument& t)
    {
      s_ -= static_cast<S>(t);
    }

    template <typename T, typename S>
    inline
    void sum<T,S>::take_as_init_(const argument& t)
    {
      s_ = static_cast<S>(t);
    }

    template <typename T, typename S>
    inline
    void
    sum<T,S>::take(const sum<T,S>& other)
    {
      s_ += other.s_;
    }

    template <typename T, typename S>
    inline
    const S&
    sum<T,S>::to_result() const
    {
      return s_;
    }

    template <typename T, typename S>
    inline
    void
    sum<T,S>::set_value(const S& s)
    {
      s_ = s;
    }

    template <typename T, typename S>
    inline
    bool
    sum<T,S>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SUM_HH
