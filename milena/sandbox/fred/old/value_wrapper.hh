// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ATTRIBUTE_VALUE_WRAPPER_HH
# define MLN_MORPHO_ATTRIBUTE_VALUE_WRAPPER_HH

/// \file mln/morpho/attribute/volume.hh
///
/// Transform an accumulator to a value (when_pix::use_v) accumulator.

# include <mln/accu/internal/base.hh>
# include <mln/util/pix.hh>

# define mln_morpho_attribute_use_v_(T) mln::morpho::attribute::value_wrapper< T >
# define mln_morpho_attribute_use_v(T) mln::morpho::attribute::value_wrapper< T >

namespace mln
{

  // Forward declaration.
  namespace morpho {
    namespace attribute {
      template <typename T> class value_wrapper;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename A>
    struct accumulator_< morpho::attribute::value_wrapper<A> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

  } // end of namespace mln::trait

  namespace morpho
  {

    namespace attribute
    {

      /// Value wrapper accumulator meta-class.
      ///
      /// The parameter \p A is the base accumulator to wrap.
      template <typename A>
      struct value_wrapper
	: public mln::accu::internal::base< typename A::result , value_wrapper<A> >
      {
	typedef typename A::argument argument;
	typedef typename A::result result;

	value_wrapper();

	/// Manipulators.
	/// \{
	void init();

	void take(const argument& v);
	template <typename I>
	void take(const util::pix<I>& px);
	void take(const value_wrapper<A>& other);

	void take_as_init(const argument& v);
	template <typename I>
	void take_as_init(const util::pix<I>& px);
	/// \}

	/// Get the value of the accumulator.
	result to_result() const;

	/// Check whether this accu is able to return a result.
	bool is_valid() const;

      protected:
	/// Delegatee accumulator.
	A accu_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename A>
      value_wrapper<A>::value_wrapper()
        : accu_()
      {
      }

      template <typename A>
      void
      value_wrapper<A>::init()
      {
	accu_.init();
      }

      template <typename A>
      void
      value_wrapper<A>::take(const argument& v)
      {
	accu_.take(v);
      }

      template <typename A>
      template <typename I>
      void
      value_wrapper<A>::take(const util::pix<I>& px)
      {
	take(px.v());
      }

      template <typename A>
      void
      value_wrapper<A>::take(const value_wrapper<A>& other)
      {
	accu_.take(other.accu_);
      }

      template <typename A>
      void
      value_wrapper<A>::take_as_init(const argument& v)
      {
	accu_.take_as_init(v);
      }

      template <typename A>
      template <typename I>
      void
      value_wrapper<A>::take_as_init(const util::pix<I>& px)
      {
	accu_.take_as_init(px);
      }

      template <typename A>
      typename value_wrapper<A>::result
      value_wrapper<A>::to_result() const
      {
	return accu_.to_result();
      }

      template <typename A>
      bool
      value_wrapper<A>::is_valid() const
      {
	return accu_.is_valid();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_VALUE_WRAPPER_HH
