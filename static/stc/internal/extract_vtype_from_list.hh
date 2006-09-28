// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/** \file stc/internal/extract_vtype_from_list.hh

    \brief Match a list of types against a set of valid patterns for
    vtype retrieval

    This file is part of the rec_get_vtype_from_list from
    stc/vtypes.hh.  */

#ifndef STC_INTERNAL_EXTRACT_VTYPE_FROM_LIST_HH
# define STC_INTERNAL_EXTRACT_VTYPE_FROM_LIST_HH

# include <mlc/flags.hh>
# include <mlc/valist.hh>

namespace stc
{

  namespace internal
  {
    // Shotcut, for readability purpose.
    typedef mlc::not_found nil ;

    /* FIXME: Currently, we don't distinguish these cases:
       - a list with no vtype,
       - a list with two vtypes or more.
       extract_vtype_from_list returns mlc::not_found for both.
       We probably want a distinction between these two cases, but
       it implies handling a lot more cases in the matching.  */

    /** Match \a list against a set of valid patterns to extract
	a vtype from it.  Return this vtype the search succeeds,
	otherwise return mlc::not_found.  */
    /** \{ */

    /*----------------.
    | Invalid cases.  |
    `----------------*/

    /// Default case: search for a single vtype failed, because there
    /// is more than one vtype.
    template <typename list>
    struct extract_vtype_from_list
    {
      typedef mlc::not_found ret;
    };

    /// Case where there is no vtype in the list.
    template <>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, nil, nil, nil, nil, nil > >
    {
      typedef mlc::not_found ret;
    };


    /*--------------.
    | Valid cases.  |
    `--------------*/

    // There is one and only one valid vtype in the following
    // patterns.

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< vtype, nil, nil, nil, nil, nil, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, vtype, nil, nil, nil, nil, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, vtype, nil, nil, nil, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, vtype, nil, nil, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, vtype, nil, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, nil, vtype, nil, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, nil, nil, vtype, nil, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, nil, nil, nil, vtype, nil > >
    {
      typedef vtype ret;
    };

    template <typename vtype>
    struct extract_vtype_from_list<
      mlc::valist_< nil, nil, nil, nil, nil, nil, nil, nil, vtype > >
    {
      typedef vtype ret;
    };
    /** \} */

  } // end of namespace stc::internal

} // end of namespace stc

#endif // ! STC_INTERNAL_EXTRACT_VTYPE_FROM_LIST_HH
