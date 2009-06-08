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

#ifndef MLN_ACCU_LOGIC_LAND_HH
# define MLN_ACCU_LOGIC_LAND_HH

/// \file mln/accu/logic/land.hh
///
/// Define a 'logical-and' accumulator.
///
/// \todo Have land be parameterized.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace accu
  {

    namespace logic
    {
      // Forward declaration.
      struct land;
    }

    namespace meta
    {
      namespace logic
      {

	/// Meta accumulator for land.
	struct land : public Meta_Accumulator< land >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::logic::land ret;
	  };
	};

      } // end of namespace mln::accu::meta::logic
    } // end of namespace mln::accu::meta

    namespace logic
    {

      /// \brief "Logical-and" accumulator.
      ///
      /// \ingroup modaccuvalues
      //
      struct land : public mln::accu::internal::base< bool, land >
      {
	typedef bool argument;

	land();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init_(const argument& t);

	void take(const argument& t);
	void take(const land& other);

	void untake(const argument& t);
	void untake(const land& other);
	/// \}

	/// Get the value of the accumulator.
	bool to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:
	unsigned nfalse_;
      };


# ifndef MLN_INCLUDE_ONLY

      inline
      land::land()
      {
	init();
      }

      inline
      void
      land::init()
      {
	nfalse_ = 0;
      }

      inline
      void land::take_as_init_(const argument& t)
      {
	nfalse_ = t ? 0 : 1;
      }

      inline
      void land::take(const argument& t)
      {
	if (t == false)
	  ++nfalse_;
      }

      inline
      void
      land::take(const land& other)
      {
	nfalse_ += other.nfalse_;
      }

      inline
      void land::untake(const argument& t)
      {
	if (t == false)
	  --nfalse_;
      }

      inline
      void
      land::untake(const land& other)
      {
	mln_precondition(other.nfalse_ <= nfalse_);
	nfalse_ -= other.nfalse_;
      }

      inline
      bool
      land::to_result() const
      {
	return nfalse_ == 0;
      }

      inline
      bool
      land::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::logic
  } // end of namespace mln::accu
} // end of namespace mln

#endif // ! MLN_ACCU_LOGIC_LAND_HH
