// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_MORPHO_ATTRIBUTE_SUM_HH
# define MLN_MORPHO_ATTRIBUTE_SUM_HH

/// \file mln/morpho/attribute/sum.hh
///
/// Define an accumulator that computes the sum of component values.

# include <mln/accu/internal/base.hh>

# include <mln/trait/value_.hh>      // For mln_sum.
# include <mln/value/builtin/all.hh> // In the case of summing builtin values.
# include <mln/literal/zero.hh>      // For initialization.

# include <mln/util/pix.hh>

namespace mln
{


  // Forward declaration.

  namespace morpho {
    namespace attribute {
      template <typename I, typename S> class sum;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I, typename S>
    struct accumulator_< morpho::attribute::sum<I,S> >
    {
      typedef accumulator::has_untake::yes	has_untake;
      typedef accumulator::has_set_value::yes	has_set_value;
      typedef accumulator::has_stop::no		has_stop;
      typedef accumulator::when_pix::use_v	when_pix;
    };

  } // end of namespace mln::trait


  namespace morpho
  {

    namespace attribute
    {

      /// Suminality accumulator class.

      template <typename I, typename S = mln_sum(mln_value(I))>
      class sum : public mln::accu::internal::base< S, sum<I,S> >
      {
	typedef mln::accu::internal::base< S, sum<I> > super_;
      public:

	typedef mln_value(I) argument;

	sum();

	/// Manipulators.
	/// \{
	void init();

	void take(const argument& v);
	void take(const util::pix<I>& px);
	void take(const sum<I,S>& other);

	void take_as_init(const argument& v);
	void take_as_init(const util::pix<I>& px);
	/// \}

	/// Get the value of the accumulator.
	S to_result() const;

	/// Check whether this accu is able to return a result.
	/// Return always true.
	bool is_valid() const;

	/// Untake a value from the accumulator.
	void untake(const argument& v);
	void untake(const util::pix<I>& px);

	/// Set the return value of the accumalator.
	void set_value(const argument& v);
	void set_value(const util::pix<I>& px);


      protected:

	/// The sum value.
	S s_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename S>
      inline
      sum<I,S>::sum()
      {
	init();
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::init()
      {
	s_ = literal::zero;
      }

      // take.

      template <typename I, typename S>
      inline
      void
      sum<I,S>::take(const argument& v)
      {
	s_ += v;
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::take(const util::pix<I>& px)
      {
	take(px.v());
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::take(const sum<I,S>& other)
      {
	s_ += other.s_;
      }

      // take_as_init.

      template <typename I, typename S>
      inline
      void
      sum<I,S>::take_as_init(const argument& v)
      {
	s_ = v;
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::take_as_init(const util::pix<I>& px)
      {
	take_as_init(px.v());
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::untake(const argument& v)
      {
	s_ -= v;
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::untake(const util::pix<I>& px)
      {
	untake(px.v());
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::set_value(const argument& v)
      {
	s_ = v;
      }

      template <typename I, typename S>
      inline
      void
      sum<I,S>::set_value(const util::pix<I>& px)
      {
	set_value(px.v());
      }

      template <typename I, typename S>
      inline
      S
      sum<I,S>::to_result() const
      {
	return s_;
      }

      template <typename I, typename S>
      inline
      bool
      sum<I,S>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_SUM_HH
