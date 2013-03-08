// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_METAL_IF_HH
# define MLN_METAL_IF_HH

/*! \file
 *
 * \brief Definition of an "if-then-else" expression type.
 */

# include <mln/metal/bool.hh>

# define mlc_if(Cond, Then, Else) typename mln::metal::if_< Cond, Then, Else >::ret



namespace mln
{

  namespace metal
  {

    namespace internal
    {

      template <bool cond, typename Then, typename Else>
      struct helper_if_;

      template <typename Then, typename Else>
      struct helper_if_< true, Then, Else >
      {
	typedef Then ret;
      };

      template <typename Then, typename Else>
      struct helper_if_< false, Then, Else >
      {
	typedef Else ret;
      };

    } // end of namespace mln::metal::internal


    /*!
      \internal
      \brief "if-then-else" expression.

      FIXME: Doc!
    */
    template <typename Cond, typename Then, typename Else>
    struct if_ : internal::helper_if_< Cond::value, Then, Else >
    {
      // ret is inherited.
    };


  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_IF_HH
