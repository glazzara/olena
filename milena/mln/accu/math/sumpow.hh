// Copyright (C) 2011 EPITA Research and Development
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

#ifndef MLN_ACCU_MATH_SUMPOW_HH
# define MLN_ACCU_MATH_SUMPOW_HH

/// \file
///
/// Define an accumulator that computes a sum of powered value.

# include <cmath>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>

# include <mln/util/pix.hh> // To prevent accu::math::sumpow to work on pixels (ambiguous).

# include <mln/trait/value_.hh>      // For mln_sum.
# include <mln/value/builtin/all.hh> // In the case of summing builtin values.
# include <mln/literal/zero.hh>      // For initialization.


namespace mln
{

  namespace accu { namespace math { template <unsigned n, typename T, typename S> struct sumpow; } };

  // Traits.
  namespace trait
  {

    template <unsigned n, typename T, typename S>
    struct accumulator_< accu::math::sumpow<n,T,S> >
    {
      typedef accumulator::has_untake::yes    has_untake;
      typedef accumulator::has_set_value::yes has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::not_ok   when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    namespace math
    {

      /// \brief Generic sum of powered value accumulator class.
      ///
      /// \ingroup modaccuvalues
      //
      template <unsigned n, typename T, typename S = mln_sum(T)>
      struct sumpow : public mln::accu::internal::base< const S&, sumpow<n,T,S> >
      {
	typedef T argument;

	sumpow();

	/// Manipulators.
	/// \{
	void init();
	void take(const argument& t);
	void take_as_init_(const argument& t);
	void take(const sumpow<n,T,S>& other);

	void untake(const argument& t);
	void untake(const sumpow<n,T,S>& other);
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


      template <unsigned n, typename I, typename S>
      struct sumpow< n, util::pix<I>, S >;


# ifndef MLN_INCLUDE_ONLY



      template <unsigned n, typename T, typename S>
      inline
      sumpow<n,T,S>::sumpow()
      {
	init();
      }

      template <unsigned n, typename T, typename S>
      inline
      void
      sumpow<n,T,S>::init()
      {
	s_ = literal::zero;
      }

      template <unsigned n, typename T, typename S>
      inline
      void sumpow<n,T,S>::take(const argument& t)
      {
	s_ += std::pow(static_cast<S>(t), n);
      }

      template <unsigned n, typename T, typename S>
      inline
      void sumpow<n,T,S>::untake(const argument& t)
      {
	s_ -= std::pow(static_cast<S>(t), n);
      }

      template <unsigned n, typename T, typename S>
      inline
      void sumpow<n,T,S>::take_as_init_(const argument& t)
      {
	s_ = std::pow(static_cast<S>(t), n);
      }

      template <unsigned n, typename T, typename S>
      inline
      void
      sumpow<n,T,S>::take(const sumpow<n,T,S>& other)
      {
	s_ += other.s_;
      }

      template <unsigned n, typename T, typename S>
      inline
      void
      sumpow<n,T,S>::untake(const sumpow<n,T,S>& other)
      {
	s_ -= other.s_;
      }

      template <unsigned n, typename T, typename S>
      inline
      const S&
      sumpow<n,T,S>::to_result() const
      {
	return s_;
      }

      template <unsigned n, typename T, typename S>
      inline
      void
      sumpow<n,T,S>::set_value(const S& s)
      {
	s_ = s;
      }

      template <unsigned n, typename T, typename S>
      inline
      bool
      sumpow<n,T,S>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::math

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MATH_SUMPOW_HH
