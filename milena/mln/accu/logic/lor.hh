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

#ifndef MLN_ACCU_LOGIC_LOR_HH
# define MLN_ACCU_LOGIC_LOR_HH

/// \file
///
/// Define a 'logical-or' accumulator.
///
/// \todo Have lor be parameterized.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  // Forward declaration.
  namespace accu {
    namespace logic {
      struct lor;
    }
  }


  // Traits.

  namespace trait
  {

    template <>
    struct accumulator_< accu::logic::lor >
    {
      typedef accumulator::has_untake::yes   has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    namespace meta
    {

      namespace logic
      {
	/// Meta accumulator for lor.
	struct lor : public Meta_Accumulator< lor >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::logic::lor ret;
	  };
	};

      } // end of namespace mln::accu::meta::logic
    } // end of namespace mln::accu::meta

    namespace logic
    {

    /// \brief "Logical-or" accumulator.
    ///
    /// \ingroup modaccuvalues
    //
    struct lor : public mln::accu::internal::base< bool, lor >
    {
      typedef bool argument;

      lor();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);

      void take(const argument& t);
      void take(const lor& other);

      void untake(const argument& t);
      void untake(const lor& other);
      /// \}

      /// Get the value of the accumulator.
      bool to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      unsigned ntrue_;
    };

# ifndef MLN_INCLUDE_ONLY

    inline
    lor::lor()
    {
      init();
    }

    inline
    void
    lor::init()
    {
      ntrue_ = 0;
    }

    inline
    void lor::take_as_init_(const argument& t)
    {
      ntrue_ = t ? 1 : 0;
    }

    inline
    void lor::take(const argument& t)
    {
      if (t == true)
	++ntrue_;
    }

    inline
    void
    lor::take(const lor& other)
    {
      ntrue_ += other.ntrue_;
    }

    inline
    void lor::untake(const argument& t)
    {
      if (t == true)
	--ntrue_;
    }

    inline
    void
    lor::untake(const lor& other)
    {
      mln_precondition(other.ntrue_ <= ntrue_);
      ntrue_ -= other.ntrue_;
    }

    inline
    bool
    lor::to_result() const
    {
      return ntrue_ != 0;
    }

    inline
    bool
    lor::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::logic
  } // end of namespace mln::accu
} // end of namespace mln


#endif // ! MLN_ACCU_LOGIC_LOR_HH
