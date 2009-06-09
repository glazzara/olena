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

#ifndef MLN_ACCU_LOGIC_LAND_BASIC_HH
# define MLN_ACCU_LOGIC_LAND_BASIC_HH

/// \file
///
/// Define a basic 'logical-and' accumulator.
///
/// \todo Have land_basic be parameterized.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace accu
  {

    namespace logic
    {
      // Forward declaration.
      struct land_basic;
    }

    namespace meta
    {

      namespace logic
      {

	/// Meta accumulator for land_basic.
	struct land_basic : public Meta_Accumulator< land_basic >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::logic::land_basic ret;
	  };
	};

      } // end of namespace mln::accu::meta::logic
    } // end of namespace mln::accu::meta


    namespace logic
    {

    /// \brief "Logical-and" accumulator.
    /// Conversely to accu::logic::land, this version does not have the 'untake'
    /// method but features the 'can_stop' method.
    ///
    /// \ingroup modaccuvalues
    //
    struct land_basic : public mln::accu::internal::base< bool, land_basic >
    {
      typedef bool argument;

      land_basic();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);

      void take(const argument& t);
      void take(const land_basic& other);
      /// \}

      /// Get the value of the accumulator.
      bool to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      /// Test if it is worth for this accumulator to take extra data.
      /// If the result is already 'false' (because this accumulator
      /// has already taken a 'false' value), can_stop returns true.
      bool can_stop() const;

    protected:
      bool res_;
    };

# ifndef MLN_INCLUDE_ONLY

    inline
    land_basic::land_basic()
    {
      init();
    }

    inline
    void
    land_basic::init()
    {
      res_ = true;
    }

    inline
    void land_basic::take_as_init_(const argument& t)
    {
      res_ = t;
    }

    inline
    void land_basic::take(const argument& t)
    {
      if (res_ == true && t == false)
	res_ = false;
    }

    inline
    void
    land_basic::take(const land_basic& other)
    {
      res_ = res_ && other.res_;
    }

    inline
    bool
    land_basic::to_result() const
    {
      return res_;
    }

    inline
    bool
    land_basic::is_valid() const
    {
      return true;
    }

    inline
    bool
    land_basic::can_stop() const
    {
      return res_ == false;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::logic
  } // end of namespace mln::accu
} // end of namespace mln


#endif // ! MLN_ACCU_LOGIC_LAND_BASIC_HH
