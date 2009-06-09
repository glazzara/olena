// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_P2B_ANTILOGY_HH
# define MLN_FUN_P2B_ANTILOGY_HH

/// \file
/// \brief Definition of a p2b function always returning \c false.

# include <mln/core/concept/function.hh>
# include <mln/core/alias/point2d.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {


      /// \brief A p2b function always returning \c false.
      ///
      /// A simpler name would be `false', but this is not a valid C++
      /// identifier, as \c false is a keyword of the language.
      struct antilogy : public Function_v2b< antilogy >
      {
	typedef bool result;

	template <typename P>
	bool operator()(const P& p) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename P>
      inline
      bool
      antilogy::operator()(const P& /* p */) const
      {
	return false;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_ANTILOGY_HH
