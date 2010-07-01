// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_COUNT_VALUE_HH
# define MLN_ACCU_COUNT_VALUE_HH

/// \file
///
/// \brief Define an accumulator that counts the occurrence of a given value.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/metal/is_a.hh>

namespace mln
{

  // Forward declaration.
  namespace accu {
    template <typename V> struct count_value;
  }


  // Traits.

  namespace trait
  {

    template <typename V>
    struct accumulator_< accu::count_value<V> >
    {
      typedef accumulator::has_untake::yes     has_untake;
      typedef accumulator::has_set_value::yes has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::use_v    when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    /// \brief Define an accumulator that counts the occurrence of a given value.
    ///
    /// \ingroup modaccuvalues
    //
    template <typename V>
    struct count_value
      : public mln::accu::internal::base< unsigned , count_value<V> >
    {
      typedef V argument;

      count_value();
      explicit count_value(const V& ref);

      /// Manipulators.
      /// \{
      void init();
      void take(const argument&);
      void take(const count_value<V>& other);
      void untake(const argument&);
      void untake(const count_value<V>& other);

      /// Force the value of the counter to \a c.
      void set_value(unsigned c);
      /// \}

      /// Get the value of the accumulator.
      unsigned to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      /// The value of the counter.
      unsigned count_;
      /// The value which must be counted.
      V ref_;

      /// Set to false if the reference value is not set.
      bool valid_;
    };


    namespace meta
    {

      /// FIXME: How to write a meta accumulator with a constructor taking
      /// a generic argument?
      /// Meta accumulator for count_value.
      struct count_value : public Meta_Accumulator< count_value >
      {

	template <typename V>
	struct with
	{
	  typedef accu::count_value<V> ret;
	};

      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    inline
    count_value<V>::count_value()
    {
      init();
      valid_ = false;
    }

    template <typename V>
    inline
    count_value<V>::count_value(const V& ref)
    {
      ref_ = ref;
      valid_ = true;
      init();
    }

    template <typename V>
    inline
    void
    count_value<V>::init()
    {
      count_ = 0;
    }

    template <typename V>
    inline
    void
    count_value<V>::take(const argument& l)
    {
      if (l == ref_)
	++count_;
    }

    template <typename V>
    inline
    void
    count_value<V>::take(const count_value<V>& other)
    {
      mln_precondition(other.is_valid());
      mln_precondition(other.ref_ == ref_);
      count_ += other.count_;
    }

    template <typename V>
    inline
    void
    count_value<V>::untake(const argument& l)
    {
      if (l == ref_)
	--count_;
    }

    template <typename V>
    inline
    void
    count_value<V>::untake(const count_value<V>& other)
    {
      mln_precondition(other.is_valid());
      mln_precondition(other.ref_ == ref_);
      count_ -= other.count_;
    }

    template <typename V>
    inline
    unsigned
    count_value<V>::to_result() const
    {
      // The background label MUST not be counted.
      return count_;
    }

    template <typename V>
    inline
    void
    count_value<V>::set_value(unsigned c)
    {
      count_ = c;
    }

    template <typename V>
    inline
    bool
    count_value<V>::is_valid() const
    {
      return valid_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNT_VALUE_HH
