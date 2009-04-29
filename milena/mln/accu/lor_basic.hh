// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_LOR_BASIC_HH
# define MLN_ACCU_LOR_BASIC_HH

/// \file mln/accu/lor_basic.hh
///
/// Define a basic 'logical-or' accumulator.
///
/// \todo Have lor_basic be parameterized.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace accu
  {

    /// "Logical-or" accumulator class.  Conversely to accu::lor,
    /// this version does not have the 'untake' method but features
    /// the 'can_stop' method.
    struct lor_basic : public mln::accu::internal::base< bool, lor_basic >
    {
      typedef bool argument;

      lor_basic();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);

      void take(const argument& t);
      void take(const lor_basic& other);
      /// \}

      /// Get the value of the accumulator.
      bool to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      /// Test if it is worth for this accumulator to take extra data.
      /// If the result is already 'true' (because this accumulator
      /// has already taken a 'true' value), can_stop returns true.
      bool can_stop() const;

    protected:
      bool res_;
    };


    namespace meta
    {

      /// Meta accumulator for lor_basic.

      struct lor_basic : public Meta_Accumulator< lor_basic >
      {
	template <typename T>
	struct with
	{
	  typedef accu::lor_basic ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    inline
    lor_basic::lor_basic()
    {
      init();
    }

    inline
    void
    lor_basic::init()
    {
      res_ = false;
    }

    inline
    void lor_basic::take_as_init_(const argument& t)
    {
      res_ = t;
    }

    inline
    void lor_basic::take(const argument& t)
    {
      if (res_ == false && t == true)
	res_ = true;
    }

    inline
    void
    lor_basic::take(const lor_basic& other)
    {
      res_ = res_ || other.res_;
    }

    inline
    bool
    lor_basic::to_result() const
    {
      return res_;
    }

    inline
    bool
    lor_basic::is_valid() const
    {
      return true;
    }

    inline
    bool
    lor_basic::can_stop() const
    {
      return res_ == true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_LOR_BASIC_HH
