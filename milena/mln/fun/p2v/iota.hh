// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_FUN_P2V_IOTA_HH
# define MLN_FUN_P2V_IOTA_HH

/// \file
///
/// Iota function.

# include <mln/core/concept/function.hh>


namespace mln
{

  namespace fun
  {

    namespace p2v
    {

      struct iota : public Function_v2v< iota >
      {
	typedef unsigned result;

	iota();

	template <typename P>
	unsigned operator()(const P&) const;

	unsigned value() const;
	void reset() const;

      protected:
	mutable unsigned i_;
      };


# ifndef MLN_INCLUDE_ONLY

      inline
      iota::iota()
      {
	reset();
      }

      template <typename P>
      inline
      unsigned
      iota::operator()(const P&) const
      {
	return ++i_;
      }

      inline
      unsigned
      iota::value() const
      {
	return i_;
      }

      inline
      void
      iota::reset() const
      {
	i_ = 0;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2V_IOTA_HH
