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

#ifndef MLN_ACCU_PAIR_HH
# define MLN_ACCU_PAIR_HH

/// \file
///
/// Define a pair of accumulators.

# include <utility>

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  namespace accu
  {


    /// \brief Generic pair of accumulators.
    ///
    /// The parameter \c T is the type of values.
    ///
    /// \todo Check that, when T is not provided, A1 and A2 have the same value.
    ///
    /// \ingroup modaccumulti
    //
    template <typename A1, typename A2, typename T = mln_argument(A1)>
    struct pair : public mln::accu::internal::base< std::pair<mln_result(A1), mln_result(A2)>,
						    pair<A1,A2,T> >
    {
      typedef T argument;

      typedef mln_result(A1) result_1;
      typedef mln_result(A2) result_2;

      pair();
// FIXME: not implemented. Do we want it?
//      pair(const A1& a1, const A2& a2);

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);
      void take(const argument& t);
      void take(const pair<A1,A2,T>& other);
      /// \}

      /// Get the value of the accumulator.
      /// \{
      std::pair<mln_result(A1), mln_result(A2)> to_result() const;
      void get_result(result_1& r1, result_2& r2) const;
      /// \}

      /// Return the result of the first accumulator.
      mln_result(A1) first() const;
      /// Return the result of the second accumulator.
      mln_result(A2) second() const;

      /// Return the first accumulator.
      A1 first_accu() const;
      /// Return the second accumulator.
      A2 second_accu() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      A1 a1_;
      A2 a2_;
    };


    namespace meta
    {

      /// Meta accumulator for pair.
      template <typename A1, typename A2>
      struct pair : public Meta_Accumulator< pair<A1,A2> >
      {
	template <typename T>
	struct with
	{
	  typedef mln_accu_with(A1, T) A1_T;
	  typedef mln_accu_with(A2, T) A2_T;
	  typedef accu::pair<A1_T, A2_T, T> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename A1, typename A2, typename T>
    inline
    pair<A1,A2,T>::pair()
    {
      init();
    }

    template <typename A1, typename A2, typename T>
    inline
    void
    pair<A1,A2,T>::init()
    {
      a1_.init();
      a2_.init();
    }

    template <typename A1, typename A2, typename T>
    inline
    void
    pair<A1,A2,T>::take_as_init_(const argument& t)
    {
      a1_.take_as_init_(t);
      a2_.take_as_init_(t);
    }

    template <typename A1, typename A2, typename T>
    inline
    void
    pair<A1,A2,T>::take(const argument& t)
    {
      a1_.take(t);
      a2_.take(t);
    }

    template <typename A1, typename A2, typename T>
    inline
    void
    pair<A1,A2,T>::take(const pair<A1,A2,T>& other)
    {
      a1_.take(other.a1_);
      a2_.take(other.a2_);
    }

    template <typename A1, typename A2, typename T>
    inline
    std::pair<mln_result(A1), mln_result(A2)>
    pair<A1,A2,T>::to_result() const
    {
      std::pair<mln_result(A1), mln_result(A2)> tmp(a1_.to_result(), a2_.to_result());
      return tmp;
    }

    template <typename A1, typename A2, typename T>
    inline
    void
    pair<A1,A2,T>::get_result(result_1& r1,
			       result_2& r2) const
    {
      r1 = a1_.to_result();
      r2 = a2_.to_result();
    }

    template <typename A1, typename A2, typename T>
    inline
    mln_result(A1)
    pair<A1,A2,T>::first() const
    {
      return a1_.to_result();
    }

    template <typename A1, typename A2, typename T>
    inline
    mln_result(A2)
    pair<A1,A2,T>::second() const
    {
      return a2_.to_result();
    }



    template <typename A1, typename A2, typename T>
    inline
    A1
    pair<A1,A2,T>::first_accu() const
    {
      return a1_;
    }

    template <typename A1, typename A2, typename T>
    inline
    A2
    pair<A1,A2,T>::second_accu() const
    {
      return a2_;
    }


    template <typename A1, typename A2, typename T>
    inline
    bool
    pair<A1,A2,T>::is_valid() const
    {
      return a1_.is_valid() && a2_.is_valid();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_PAIR_HH
