// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_INTERNAL_COUPLE_HH
# define MLN_ACCU_INTERNAL_COUPLE_HH

/// \file
///
/// Base implementation of a couple of accumulators.

# include <utility>

# include <mln/core/concept/accumulator.hh>
# include <mln/accu/internal/base.hh>

# include <mln/metal/equal.hh>


namespace mln
{

  namespace accu
  {

    namespace internal
    {

      /// Base implementation of a couple of accumulators.
      ///
      /// The parameter \c T is the type of values.
      ///
      /// \todo Check that, when T is not provided, A1 and A2 have the same value.
      template <typename A1, typename A2, typename R, typename E>
      class couple
        : public base<R,E>,
	  public mlc_equal(mln_argument(A1), mln_argument(A2))::check_t
      {
	public:
        typedef mln_argument(A1) argument;

	/// Manipulators.
	/// \{
	void init();
	void take_as_init(const argument& t);
	void take(const argument& t);
	void take(const E& other);
	/// \}

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	A1& first();
	A2& second();
	const A1& first() const;
	const A2& second() const;

	protected:
	couple();

	A1 a1_;
	A2 a2_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename A1, typename A2, typename R, typename E>
      inline
      couple<A1,A2,R,E>::couple()
      {
	init();
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      void
      couple<A1,A2,R,E>::init()
      {
	a1_.init();
	a2_.init();
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      void
      couple<A1,A2,R,E>::take_as_init(const argument& t)
      {
	a1_.take_as_init(t);
	a2_.take_as_init(t);
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      void
      couple<A1,A2,R,E>::take(const argument& t)
      {
	a1_.take(t);
	a2_.take(t);
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      void
      couple<A1,A2,R,E>::take(const E& other)
      {
	a1_.take(other.a1_);
	a2_.take(other.a2_);
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      bool
      couple<A1,A2,R,E>::is_valid() const
      {
	return a1_.is_valid() && a2_.is_valid();
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      A1&
      couple<A1,A2,R,E>::first()
      {
	return a1_;
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      const A1&
      couple<A1,A2,R,E>::first() const
      {
	return a1_;
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      A2&
      couple<A1,A2,R,E>::second()
      {
	return a2_;
      }

      template <typename A1, typename A2, typename R, typename E>
      inline
      const A2&
      couple<A1,A2,R,E>::second() const
      {
	return a2_;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::internal

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_INTERNAL_COUPLE_HH
